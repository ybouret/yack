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
#include "yack/sort/indexing.hpp"

#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        static inline void set_not_conserved(imatrix &NuA, const size_t j) throw()
        {
            for(size_t i=NuA.rows;i>0;--i) NuA[i][j] = 0;
        }



        static inline size_t get_coeff_count(const readable<int> &nu) throw()
        {
            size_t res = 0;
            for(size_t j=nu.size();j>0;--j)
            {
                if(0!=nu[j]) ++res;
            }
            return res;
        }


        
        typedef meta_list<const equilibrium> ep_group_; //!< list of pointers
        typedef ep_group_::node_type          ep_node;  //!< nodes for ep_list
        
        //----------------------------------------------------------------------
        //
        //! (linked) group of equilibria
        //
        //----------------------------------------------------------------------
        class ep_group : public object, public ep_group_
        {
        public:
            inline explicit ep_group() throw() : object(), ep_group_(), next(0), prev(0) {}
            inline virtual ~ep_group() throw() {}

            
            //! display
            inline friend std::ostream & operator<<(std::ostream &os, const ep_group &self)
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
            
            //! detect adjacent by NuA
            inline bool linked_to(const equilibrium &eq, const imatrix &NuA) const throw()
            {
                assert(size>0);
                const readable<int> &lhs = NuA[*eq];
                for(const ep_node *en=head;en;en=en->next)
                {
                    const equilibrium   &me  = **en;
                    const readable<int> &rhs = NuA[*me];
                    if(linkedRows(lhs,rhs)) {
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
        
        //----------------------------------------------------------------------
        //
        //! high level list of groups
        //
        //----------------------------------------------------------------------
        class ep_groups : public cxx_list_of<ep_group>
        {
        public:
            explicit ep_groups() throw() : cxx_list_of<ep_group>() {}
            virtual ~ep_groups() throw() {}

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ep_groups);
        };


        
        static inline
        bool are_all_geqz(const readable<apz> &q)
        {
            for(size_t j=q.size();j>0;--j)
            {
                if(q[j]<0) {
                    return false;
                }
            }
            return true;
        }

        typedef vector<apz>      zvec;
        typedef shared_ptr<zvec> zvec_ptr;
        typedef vector<zvec_ptr> zstore_;

        class zstore : public zstore_
        {
        public:
            explicit zstore() throw() : zstore_() {}
            virtual ~zstore() throw() {}

            bool grow(const zvec &rhs)
            {
                for(size_t i=size();i>0;--i)
                {
                    const readable<apz> &lhs = *((*this)[i]);
                    assert(rhs.size()==lhs.size());

                    if( apk::are_prop(lhs,rhs,NULL) )
                    {
                        return false;
                    }
                }

                const zvec_ptr zp = new zvec(rhs);
                push_back(zp);
                return true;
            }


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(zstore);
        };



        static inline
        void sparsity_detect(zstore              &pos,
                             const readable<apz> &lhs,
                             const readable<apz> &rhs,
                             const xmlog         &xml)
        {

            static const unsigned has_pos = 0x01;
            static const unsigned has_neg = 0x02;
            static const unsigned has_mix = (has_pos|has_neg);
            
            const size_t dims = lhs.size(); // dimension
            vector<apz>  vtry(dims);        // trial vector
            
            // loop over all pair of coefficients
            for(size_t k=1;k<=dims;++k)
            {
                const apz &l = lhs[k];
                const apz &r = rhs[k];
                if(0==l||0==r)
                {
                    continue;
                }

                const apz a     =  r;
                const apz b     = -l;
                unsigned  flags = 0x00;
                size_t    count = 0;
                for(size_t i=dims;i>0;--i)
                {
                    switch( (vtry[i] = a * lhs[i] + b*rhs[i]).s )
                    {
                        case __zero__: break;

                        case positive:
                            flags |= has_pos;
                            ++count;
                            break;

                        case negative:
                            flags |= has_neg;
                            ++count;
                            break;
                    }
                }

                if(count<=1) continue;

                
                switch(flags)
                {
                    case has_mix:
                        // mixed signs vector
                        continue;

                    case has_pos:
                        assert(are_all_geqz(vtry));
                        break;

                    case has_neg:
                        iota::neg(vtry);
                        assert(are_all_geqz(vtry));
                        break;

                    default:
                        // zero vector
                        continue;
                }

                apk::simplify(vtry);
                if(pos.grow(vtry))
                {
                    YACK_XMLOG(xml,"(+) " << pos.back());
                }

            }
        }

        
        static inline
        void sparsity_build(matrix<unsigned> &w, const matrix<apq> &Q, const xmlog &xml)
        {
            const size_t rows = Q.rows;
            const size_t dims = Q.cols;
            zstore       pos; // pos
            zstore       neg; // neg
            
            assert(0==w.rows);
            assert(0==w.cols);
            
            YACK_XMLOG(xml,"-- initialize rows");
            for(size_t i=1;i<=rows;++i)
            {
                zvec z(dims);
                {
                    const readable<apq> &q = Q[i];
                    for(size_t j=dims;j>0;--j)
                    {
                        z[j] = q[j].num;
                    }
                }
                
                if(are_all_geqz(z))
                {
                    // dispatch into positive repo
                    if(pos.grow(z))
                    {
                        YACK_XMLOG(xml,"(+) " << pos.back());
                    }
                }
                else
                {
                    // dispatch into negative
                    if(neg.grow(z))
                    {
                        YACK_XMLOG(xml,"(-) " << neg.back());
                    }
                }
            }

            YACK_XMLOG(xml,"-- finding combinations");
            const size_t nn = neg.size();
            for(size_t i=1;i<=nn;++i)
            {
                // taking a negative
                const readable<apz> &lhs = *neg[i];

                // testing it against all remaining negative
                for(size_t j=i+1;j<=nn;++j)
                {
                    const readable<apz> &rhs = *neg[j];
                    sparsity_detect(pos,lhs,rhs,xml);
                }

                // testing it against all newly created positive
                for(size_t j=pos.size();j>0;--j)
                {
                    const readable<apz> &rhs = *pos[j];
                    sparsity_detect(pos,lhs,rhs,xml);
                }
            }
            const size_t   np = pos.size();
            if(np<=0)
            {
                YACK_XMLOG(xml,"-- no possibility");
                return;
            }
            

            YACK_XMLOG(xml,"-- computing cost of #" << np);
            vector<apn>    wp(np,as_capacity);
            for(size_t i=1;i<=np;++i)
            {
                const readable<apz> &z = *pos[i];
                apn    sum = 0;
                for(size_t j=dims;j>0;--j)
                    sum += z[j].n;
                wp << sum;
            }

            vector<size_t> ip(np);
            indexing::make(ip,comparison::increasing<apn>,wp);
            
            
            YACK_XMLOG(xml,"-- gathering matrix");
            matrix<apq> W(np,dims);

            for(size_t i=1;i<=np;++i)
            {
                const size_t ii = ip[i];
                std::cerr << "\tZ" << i << " = " << *pos[ii] << " //  " << wp[ii] << std::endl;
                iota::load(W[ii],*pos[ii]);
            }

            YACK_XMLOG(xml,"-- computing rank...");
            const size_t rank = apk::gj_rank(W);
            YACK_XMLOG(xml,"-- rank = " << rank);

            
            if(rank)
            {
                w.make(rank,dims);
                size_t i = 0;
                size_t k = 0;
                while(i<rank)
                {
                    ++i;
                TRY_AGAIN:
                    ++k;
                    const readable<apz> &p = *pos[ ip[k] ];
                    for(size_t j=dims;j>0;--j)
                    {
                        w[i][j] = p[j].cast_to<unsigned>();
                    }
                    if( apk::gj_rank_of(w) < i)
                    {
                        goto TRY_AGAIN;
                    }
                    else
                    {
                        std::cerr << "+ " << p << std::endl;
                    }
                }
            }
            
            std::cerr << "w=" << w << std::endl;

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
            ep_group          edb;
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
                        //goto DROP;
                    }


                    YACK_XMLOG(xml, "|_[keep]");
                    sdb << & coerce(s);
                    continue;

                DROP:
                    YACK_XMLOG(xml,"|_[drop]");
                    set_not_conserved(NuA,j);
                }

                std::cerr << "Nu =" << Nu  << std::endl;
                std::cerr << "NuA=" << NuA << std::endl;

                exit(0);

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
                    if(get_coeff_count(NuA[*eq])>0)
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
                    assert( get_coeff_count(NuA[*eq])>0 );

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
                        YACK_XMLOG(xml, "\t" << nu);
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
                    // COMPACT: record each species and its indices
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

                        if(verbose) std::cerr << "\tP = " << P << std::endl;
                        if(!lu.build(iP2)) throw exception("singular topology!!");
                        const apq  detP2 = lu.determinant(iP2); assert(0!=detP2);
                        matrix<apq> adjP2(rows,rows);
                        lu.adjoint(adjP2,P2);

                        matrix<apq> adjP3(rows,cols); iota::mmul(adjP3,adjP2,P);
                        matrix<apq> invP(cols,cols);  iota::mmul(invP,Pt,adjP3);


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
                    apk::simplify_rows(Q);
                    if(verbose) std::cerr << "\tQ = " << Q << std::endl;

                    //----------------------------------------------------------
                    //
                    // computing possible weights
                    //
                    //----------------------------------------------------------
                    matrix<unsigned>  weights;
                    sparsity_build(weights,Q,xml);
                    const size_t nc = weights.rows;

                    // decompacting
                    if(nc>0)
                    {
                        //matrix<unsigned> F(nc,M);

                        for(size_t i=1;i<=nc;++i)
                        {
                            const readable<unsigned> &weight = weights[i];
                            assert(weight.size()==cols);
                            constraint A = new conserve();
                            for(size_t j=1;j<=cols;++j)
                            {
                                const unsigned w = weight[j];
                                if(w)
                                {
                                    const species &s = *spdb[j];
                                    (*A)(s,w);
                                    //F[i][*s] = w;
                                }
                            }
                            std::cerr << "constraint: d(" << A << ")=0" << std::endl;
                            cnsv.push_back(A);
                        }

                        //std::cerr << "F="  << F  << std::endl;
                        //std::cerr << "Nu=" << Nu << std::endl;

                    }




                }


            }

            std::cerr << "#constraints=" << cnsv.size() << std::endl;

            matrix<unsigned> F(cnsv.size(),M);
            for(size_t i=1;i<=cnsv.size();++i)
            {
                for(const actor *a=(*cnsv[i])->head;a;a=a->next)
                {
                    F[i][***a] = a->nu;
                }
            }

            std::cerr << "F =" << F  << std::endl;
            std::cerr << "Nu=" << Nu << std::endl;


            exit(0);
            // "@eq:-[a]+4[b]+7[c]-2[d]:1"
        }

    }

}

