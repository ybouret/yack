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

#include "yack/apex/kernel.hpp"
#include "yack/math/algebra/crout.hpp"
#include "yack/counting/comb.hpp"
#include "yack/counting/perm.hpp"
#include "yack/sequence/list.hpp"
#include "yack/ptr/shared.hpp"

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



        static inline size_t coeffCount(const readable<int> &nu) throw()
        {
            size_t res = 0;
            for(size_t j=nu.size();j>0;--j)
            {
                if(0!=nu[j]) ++res;
            }
            return res;
        }


        typedef meta_list<const equilibrium> ep_list_;
        typedef ep_list_::node_type          ep_node;

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



        // simplify by GCD of positive numerators
        static inline
        void simplifyRow(writable<apq> &r)
        {
            const size_t n = r.size(); assert(n>0);
            vector<apn> value(n,as_capacity);
            for(size_t i=n;i>0;--i)
            {
                const apn &u = r[i].num.n; assert(1==r[i].den);
                if(u>0) value.push_back(u);
            }

            switch(value.size())
            {
                case 0:
                    return;

                case 1:
                    iota::div_by(value.front(),r);
                    return;

                default:
                    break;
            }

            apn g = apn::gcd(value[1], value[2]);
            for(size_t i=value.size();i>2;--i)
            {
                g = apn::gcd(g,value[i]);
            }
            iota::div_by(g,r);
        }

#if 1
        // simplify all rows
        static inline
        void simplifyRows(matrix<apq> &r)
        {
            for(size_t i=r.rows;i>0;--i)
                simplifyRow(r[i]);
        }
#endif

        size_t transformQ(matrix<apq> &Q)
        {
            const apq    _0   = 0;
            const size_t n    = Q.rows;
            size_t       rank = 0;
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
                //std::cerr << "ipiv=" << ipiv << ", piv=" << qpiv << std::endl;
                if(apiv<=0)
                {
                    for(size_t j=i+1;j<=n;++j)
                    {
                        Q[j].ld(_0);
                    }
                    goto DONE;
                }
                ++rank;
                if(i!=ipiv)
                {
                    Q.swap_rows(i,ipiv);
                }


                for(size_t k=i+1;k<=n;++k)
                {
                    const apq f = Q[k][i] / qpiv;
                    for(size_t j=n;j>i;--j) Q[k][j] -= f * Q[i][j];
                    Q[k][i] = 0;
                }
            }

        DONE:
            for(size_t i=1;i<=rank;++i)
            {
                writable<apq> &Qi = Q[i];
                const apn      l  = apk::lcm(Qi);
                iota::mul_by(l,Qi);
                simplifyRow(Qi);

                apq    amax = abs_of(Qi[1]);
                size_t jmax = 1;
                for(size_t j=2;j<=n;++j)
                {
                    const apq atmp = abs_of(Qi[j]);
                    if(atmp>amax)
                    {
                        amax = atmp;
                        jmax = j;
                    }
                }
                assert(amax>0);
                if(Qi[jmax]<0) iota::neg(Qi);
                std::cerr << "Q" << i << " = " << Qi << std::endl;
            }
            return rank;
        }


#if 0
        static inline
        size_t orthoQ( matrix<apq> &Q )
        {
            const size_t n    = Q.rows;
            size_t       rank = 0;
            const apq   _0    = 0;
            vector<apq>  u_k(n,_0);
            vector<apq>  tmp(n,_0);
            std::cerr << "u1 = " << Q[1] << std::endl;
            for(size_t k=2;k<=n;++k)
            {
                const readable<apq> &v_k = Q[k];
                iota::load(u_k,v_k);
                for(size_t j=1;j<k;++j)
                {
                    const readable<apq> &u_j = Q[j];
                    const apq            den = iota::dot<apq>::of(u_j,u_j);
                    const apq            num = iota::dot<apq>::of(v_k,u_j);
                    const apq            fac = num/den;
                    iota::load(tmp,u_j);
                    for(size_t i=n;i>0;--i)
                    {
                        u_k[i] -= fac * tmp[i];
                    }
                }
                iota::load(Q[k],u_k);
                const apn l = apk::lcm(Q[k]);
                iota::mul_by(l,Q[k]);
                simplifyRow(Q[k]);
                std::cerr << "u" << k <<  " = "  << Q[k] << std::endl;

                if( iota::dot<apq>::of(Q[k],Q[k]) <= 0)
                {
                    for(size_t j=k+1;j<=n;++j)
                    {
                        Q[j].ld(_0);
                    }
                    break;
                }

                ++rank;
            }
            return rank;
        }
#endif


        bool allPos(const readable<apq> &q)
        {
            for(size_t j=q.size();j>0;--j)
            {
                if(q[j]<0) {
                    return false;
                }
            }
            return true;
        }

        typedef vector<apq>      qvec;
        typedef shared_ptr<qvec> qvec_ptr;
        typedef vector<qvec_ptr> qstore;

        static inline
        void buildConstraintsFrom(const matrix<apq> &Q, const size_t rank)
        {
            qstore  p; // pos
            qstore  n; // neg

            //initialize
            for(size_t i=1;i<=rank;++i)
            {
                const qvec_ptr q = new qvec(Q[i],transmogrify);
                if(allPos(*q))
                {
                    std::cerr << " (+) " << q << std::endl;
                    p.push_back(q);
                }
                else
                {
                    std::cerr << " (-) " << q << std::endl;
                    n.push_back(q);
                }
            }

            // sparse comnination
            for(size_t i=1;i<rank;++i)
            {
                const readable<apq> &lhs = Q[i];
                for(size_t j=i+1;j<=rank;++j)
                {
                    const readable<apq> &rhs = Q[j];
                    std::cerr << "testing " << lhs << "/" << rhs << std::endl;
                }
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
                    const species &s = **an;          // get species
                    const size_t   j = *s;            // get index
                    const islot   &l = held_by[j];    // get holding equilibria

                    if(verbose) corelib.pad(*xml<< s.name,s) << " : " << l << std::endl;
                    assert(s.rank==l.size);
                    assert(s.rank>0);

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
            //   collect remaining equilibria within POSSIBLE ones
            //
            //------------------------------------------------------------------
            {
                YACK_XMLSUB(xml, "collectingEquilibria");
                assert( (*adb).size > 0 );

                for(addrbook::const_iterator it=adb.begin();it!=adb.end();++it)
                {
                    const equilibrium &eq   = *static_cast<const equilibrium *>(*it);
                    if(coeffCount(NuA[*eq]))
                    {
                        // remaining species
                        YACK_XMLOG(xml, "-- [+] <" << eq.name << ">");
                        edb << & coerce(eq);
                    }
                    else
                    {
                        // was emptied of its species
                        YACK_XMLOG(xml, "-- [-] <" << eq.name << ">");
                    }
                }
            }


            singles.graphviz("singles.dot",corelib);

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
                if(verbose) for(const ep_group *g=groups.head;g;g=g->next) *xml << "-- " << *g << std::endl;
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
                    // record each species and its indices
                    //
                    //----------------------------------------------------------
                    const size_t      cols = (*adb).size;
                    vector<size_t>    jcol(cols,as_capacity);
                    vector<species *> spdb(cols,as_capacity);
                    for(addrbook::const_iterator it=adb.begin();it!=adb.end();++it)
                    {
                        const species &s  = *static_cast<const species *>(*it);
                        spdb << & coerce(s);
                        jcol << * s;
                        assert(spdb.size()==jcol.size());
                    }


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
                    const size_t rank = cols-rows;
                    YACK_XMLOG(xml, "\t|rank| = " << rank);

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
                    const size_t kmin = cols+1-rank; assert(kmin>=2);
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

                        std::cerr << "\tP = " << P << std::endl;
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
                    simplifyRows(Q);
                    std::cerr << "\tQ = " << Q << std::endl;

                    if(rank != transformQ(Q) )
                    {
                        throw exception("%s: unable to decompose orthogonal space",fn);
                    }
                    std::cerr << "\tQ = " << Q << std::endl;

                    buildConstraintsFrom(Q,rank);


                }




            }



            exit(0);
            // "@eq:-[a]+4[b]+7[c]-2[d]:1"
        }

    }

}

