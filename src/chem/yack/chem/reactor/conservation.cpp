#include "yack/chem/reactor.hpp"
#include "yack/chem/eqs/composite.hpp"
#include "yack/system/imported.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/signs.hpp"
#include "yack/associative/addrbook.hpp"
#include "yack/math/iota.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/type/boolean.h"
#include "yack/type/utils.hpp"

#include "yack/apex.hpp"
#include "yack/math/algebra/crout.hpp"
#include "yack/counting/comb.hpp"
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        static inline void notConserved(imatrix &NuA, const size_t j) throw()
        {
            for(size_t i=NuA.rows;i>0;--i) NuA[i][j] = 0;
        }



        static inline apq negativeMin(const readable<apq> &arr)
        {
            apq res = 0;
            for(size_t i=arr.size();i>0;--i)
            {
                const apq &tmp = arr[i];
                if(tmp<0&&tmp<res) res=tmp;
            }
            return res;
        }

        static inline size_t coeffCount(const readable<int> &nu) throw()
        {
            size_t res = 0;
            for(size_t j=nu.size();j>0;--j)
            {
                if(0!=nu[j]) ++res;
            }
            return res;
        }



        static inline apq ObjFcn(const apq           &f,
                                 writable<apq>       &trial,
                                 writable<apq>       &coeff,
                                 const readable<apq> &alpha,
                                 const readable<apq> &delta,
                                 const matrix<apq>   &P)
        {
            const size_t n = trial.size();
            for(size_t i=n;i>0;--i)
            {
                trial[i] = (f*alpha[i]+delta[i]); assert(trial[i]>=0);
            }
            //std::cerr << f << "*" << alpha << " + " << delta << std::endl;
            iota::mul(coeff,P,trial);

            return iota::mod2<apq>::of(coeff)/(f*f);

        }


        

        typedef meta_list<const equilibrium> ep_list_;
        typedef ep_list_::node_type           ep_node;

        class ep_list : public ep_list_
        {
        public:
            inline explicit ep_list() throw() : ep_list_() {}
            inline virtual ~ep_list() throw() {}

            inline friend std::ostream & operator<<(std::ostream &os, const ep_list &self)
            {
                os << "{ ";
                const ep_node *node=self.head;
                if(node)
                {
                    os << (**node).name;
                    for(node=node->next;node;node=node->next)
                    {
                        os << ", " << (**node).name;
                    }
                }
                os << " }";
                return os;
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ep_list);
        };

        
        class ep_group : public object, public ep_list
        {
        public:
            explicit ep_group() throw() : object(), ep_list(), next(0), prev(0) {}
            virtual ~ep_group() throw() {}

            bool linked_to(const equilibrium &eq, const imatrix &NuA) const throw()
            {
                assert(size>0);
                const readable<int> &lhs = NuA[*eq];
                for(const ep_node *en=head;en;en=en->next)
                {
                    const equilibrium   &me  = **en;
                    const readable<int> &rhs = NuA[*me];
                    if(linkedRows(lhs,rhs))
                    {
                        return true;
                    }

                }
                return false;
            }

            ep_group *next;
            ep_group *prev;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ep_group);
            static inline bool linkedRows(const readable<int> &lhs,
                                          const readable<int> &rhs) throw()
            {
                assert(lhs.size()==rhs.size());
                for(size_t j=lhs.size();j>0;--j)
                {
                    if( lhs[j] && rhs[j] ) return true;
                }
                return false;
            }
        };

        typedef cxx_list_of<ep_group> ep_groups_;

        class ep_groups : public ep_groups_
        {
        public:
            explicit ep_groups() throw() : ep_groups_() {}
            virtual ~ep_groups() throw() {}

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ep_groups);
        };



        static inline
        void simplifyAlpha(writable<apq> &alpha, const readable<size_t> &comb)
        {

            const size_t k = comb.size();
            apn          g = apn::gcd( alpha[comb[1]].num.n, alpha[ comb[2] ].num.n );
            for(size_t i=3;i<=k;++i)
            {
                g = apn::gcd(g,alpha[comb[i]].num.n);
            }
            iota::div_by(g,alpha);
        }

        void transformQ(matrix<apq> &Q)
        {
            const size_t n = Q.rows;
            for(size_t i=1;i<=n;++i)
            {
                apq    qpiv = Q[i][i];
                apq    apiv = abs_of(qpiv);
                size_t ipiv = i;
                for(size_t k=i+1;k<=n;++k)
                {
                    const apq qtmp = Q[k][i];
                    const apq atmp = abs_of(qtmp);
                    if(atmp>apiv)
                    {
                        ipiv=k;
                        qpiv=qtmp;
                        apiv=atmp;
                    }
                }
                std::cerr << "ipiv=" << ipiv << std::endl;
            }
        }


        void reactor:: conservation(const xmlog &xml)
        {
            static const char fn[] = "conservation";
            YACK_XMLSUB(xml,fn);


            //------------------------------------------------------------------
            //
            //
            // initialize
            //
            //
            //------------------------------------------------------------------
            NuA.assign(Nu);
            ep_list           edb;
            addrbook          adb;
            //------------------------------------------------------------------
            //
            // checking possible species and equilibria
            //
            //------------------------------------------------------------------
            {
                YACK_XMLSUB(xml, "collectingSpecies");
                vector<species *> sdb(working.size,as_capacity);
                for(const anode *an=working.head;an;an=an->next)
                {
                    const species &s = **an;        assert(s.rank>0);
                    const size_t   j = *s;
                    const islot   &l = held_by[j];

                    if(verbose) corelib.pad(*xml<< s.name,s) << " : " << l << std::endl;
                    assert(s.rank==l.size);

                    size_t  n    = 0;    // number of negative coeff(s)
                    size_t  p    = 0;    // number of positive coeff(s)

                    for(const inode *en=l.head;en;en=en->next)
                    {
                        const equilibrium &eq = **en;

                        //----------------------------------------------------------
                        // check kind of equilibrium
                        //----------------------------------------------------------
                        switch(eq.kind())
                        {
                            case undefined: throw exception("%s: undefined <%s>",fn,eq.name());
                            case part_only:
                            case join_only:
                                YACK_XMLOG(xml,"|_in one way <" << eq.name << ">");
                                goto DROP;
                            case both_ways:
                                // register in POSSIBLE eqs
                                adb.ensure(&eq);
                                break;
                        }

                        //----------------------------------------------------------
                        // check coefficient accros Nu row
                        //----------------------------------------------------------
                        switch( __sign::of(Nu[*eq][j]) )
                        {
                            case __zero__:
                                throw exception("%s: unused '%s' in <%s>", fn, s.name(), eq.name() );
                                continue;

                            case positive:
                                ++p;
                                break;

                            case negative:
                                ++n;
                                break;
                        }
                    }

                    YACK_XMLOG(xml,"|_[" << p << "+|" << n << "-]");

                    if(p>1||n>1)
                    {
                        //------------------------------------------------------
                        // not a conservative equilibrium
                        //------------------------------------------------------
                        YACK_XMLOG(xml,"|_shared");
                        goto DROP;
                    }


                    YACK_XMLOG(xml, "|_[keep]");
                    sdb << & coerce(s);
                    continue;

                DROP:
                    YACK_XMLOG(xml,"|_[drop]");
                    notConserved(NuA,j);
                }

                std::cerr << "NuA=" << NuA << std::endl;

                if(!sdb.size())
                {
                    YACK_XMLOG(xml, "-- no conserved species is detected...");
                    return;
                }

            }

            //------------------------------------------------------------------
            //
            // finally collect remaining equilibria
            //
            //------------------------------------------------------------------
            {
                YACK_XMLSUB(xml, "collectingEquilibria");
                assert( (*adb).size > 0 );

                // retrieve equilibria
                for(addrbook::const_iterator it=adb.begin();it!=adb.end();++it)
                {
                    const void        *addr = *it; assert(NULL!=addr);
                    const equilibrium &eq   = *static_cast<const equilibrium *>(addr);
                    if(coeffCount(NuA[*eq]))
                    {
                        YACK_XMLOG(xml, "-- [+] <" << eq.name << ">");
                        edb << & coerce(eq);
                    }
                    else
                    {
                        YACK_XMLOG(xml, "-- [-] <" << eq.name << ">");
                    }
                }
            }




            singles.graphviz("singles.dot",corelib);
            //lattice.graphviz("lattice.dot",worklib);

            //------------------------------------------------------------------
            //
            //
            // Grouping filtered equilibria, using NuA connectivity
            //
            //
            //------------------------------------------------------------------
            ep_groups groups;
            {
                YACK_XMLSUB(xml, "creatingGroups");

                //--------------------------------------------------------------
                //
                // loop over filtered eqs
                //
                //--------------------------------------------------------------
                while(edb.size)
                {
                    auto_ptr<ep_node>    en  = edb.pop_front();
                    const equilibrium   &eq  = **en;
                    assert( both_ways == eq.kind() );
                    assert( coeffCount(NuA[*eq])>0 );

                    //----------------------------------------------------------
                    // look for a connex group
                    //----------------------------------------------------------
                    ep_group     *grp = NULL;
                    for(ep_group *g=groups.head;g;g=g->next)
                    {
                        if(g->linked_to(eq,NuA)) {
                            grp = g;
                            break;
                        }
                    }
                    //----------------------------------------------------------
                    // if no connex group was found, build a new one
                    //----------------------------------------------------------
                    if(!grp) groups.push_back( grp = new ep_group() );


                    //----------------------------------------------------------
                    // record equilibrium in current group
                    //----------------------------------------------------------
                    grp->push_back( en.yield() );
                    YACK_XMLOG(xml, "--> " << *grp);

                }

                YACK_XMLOG(xml, "--> |group| = " << groups.size);
                if(verbose)
                {
                    for(const ep_group *g=groups.head;g;g=g->next)
                    {
                        *xml << "-- " << *g << std::endl;
                    }
                }

            }



            //------------------------------------------------------------------
            //
            //
            // creating constraints
            //
            //
            //------------------------------------------------------------------
            if(groups.size>0)
            {
                YACK_XMLSUB(xml,"creatingConstraints");
                const apq _0 = 0;
                const apq _1 = 1;


                //----------------------------------------------------------
                //
                // loop over each group of connex equilibria
                //
                //----------------------------------------------------------
                for(const ep_group *grp=groups.head;grp;grp=grp->next)
                {
                    YACK_XMLSUB(xml,"newConstraint");

                    //----------------------------------------------------------
                    //
                    // collect all involved species from one group
                    //
                    //----------------------------------------------------------
                    YACK_XMLOG(xml, "-- using " << *grp);
                    adb.free();
                    const size_t rows = grp->size; assert(rows>0);
                    for(const ep_node *ep=grp->head;ep;ep=ep->next)
                    {
                        const equilibrium   &eq = **ep;
                        const size_t         ei =  *eq;
                        const readable<int> &nu = NuA[ei];
                        if(verbose) *xml << "\t" << nu << std::endl;
                        for(const cnode *cn=eq.head();cn;cn=cn->next)
                        {
                            const species &s = ****cn;
                            const size_t   j = *s;
                            if(nu[j]) adb.ensure(&s);
                        }
                    }
                    assert((*adb).size>0);

                    //----------------------------------------------------------
                    //
                    // record species and indices
                    //
                    //----------------------------------------------------------
                    const size_t      cols = (*adb).size;
                    vector<size_t>    jcol(cols,as_capacity);
                    vector<species *> spdb(cols,as_capacity);
                    for(addrbook::const_iterator it=adb.begin();it!=adb.end();++it)
                    {
                        const void    *p  = *it; assert(NULL!=p);
                        const species &s  = *static_cast<const species *>(p);
                        spdb << & coerce(s);
                        jcol << *s;
                    }
                    assert(spdb.size()==jcol.size());

                    if(verbose) {
                        *xml << "\t|rows| = " << rows << std::endl;
                        *xml << "\t|cols| = " << cols << ':';
                        for(size_t j=1;j<=cols;++j)
                        {
                            std::cerr << ' ' << spdb[j]->name;
                        }
                        std::cerr << std::endl;
                    }


                    //----------------------------------------------------------
                    //
                    // check status
                    //
                    //----------------------------------------------------------
                    if(rows>=cols)
                    {
                        YACK_XMLOG(xml, "\tno constraint");
                        continue;
                    }
                    const size_t cons = cols-rows;
                    YACK_XMLOG(xml, "\t|cons| = " << cons);

                    //----------------------------------------------------------
                    //
                    // create constraints SUB-matrix
                    //
                    //----------------------------------------------------------
                    matrix<apq> P(rows,cols);
                    {
                        size_t irow = 0;
                        for(const ep_node *ep=grp->head;ep;ep=ep->next)
                        {
                            const equilibrium   &eq = **ep;
                            const size_t         ei =  *eq;
                            const readable<int> &nu = NuA[ei];
                            writable<apq>       &Pi = P[++irow];
                            for(size_t j=cols;j>0;--j)
                            {
                                Pi[j] = nu[ jcol[j] ];
                            }
                        }
                    }

                    //----------------------------------------------------------
                    //
                    // computing possible co-dimensions
                    //
                    //----------------------------------------------------------
                    const size_t kmin = cols+1-cons; assert(kmin>=2);
                    const size_t kmax = cols;
                    YACK_XMLOG(xml,"\t|dims| = " << kmin << " -> " << kmax);

                    //----------------------------------------------------------
                    //
                    // computing orthogonal vectors
                    //
                    //----------------------------------------------------------
                    matrix<apq> Q(cols,cols);
                    {
                        matrix<apq> Pt(P,transposed);
                        matrix<apq> P2(rows,rows);
                        iota::mmul(P2,P,Pt);
                        matrix<apq> iP2(P2);
                        crout<apq>  lu(rows);

                        std::cerr << "P = " << P << std::endl;
                        if(!lu.build(iP2)) throw exception("singular topology!!");
                        const apq  detP2 = lu.determinant(iP2); assert(0!=detP2);
                        matrix<apq> adjP2(rows,rows);
                        lu.adjoint(adjP2,P2);

                        matrix<apq> adjP3(rows,cols);
                        iota::mmul(adjP3,adjP2,P);
                        matrix<apq> invP(cols,cols);
                        iota::mmul(invP,Pt,adjP3);

                        for(size_t i=cols;i>0;--i)
                        {
                            for(size_t j=cols;j>0;--j)
                            {
                                if(i==j)
                                {
                                    Q[i][i] = detP2 - invP[i][i];
                                }
                                else
                                {
                                    Q[i][j] = - invP[j][i];
                                }
                            }
                        }
                    }
                    std::cerr << "Q = " << Q << std::endl;
                    transformQ(Q);
                }




            }



            exit(0);
            // "@eq:-[a]+4[b]+7[c]-2[d]:1"
        }

    }

}

