#include "yack/chem/reactor.hpp"
#include "yack/chem/eqs/composite.hpp"
#include "yack/system/imported.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/signs.hpp"
#include "yack/associative/addrbook.hpp"
#include "yack/math/iota.hpp"
#include "yack/ptr/auto.hpp"

#include "yack/apex.hpp"
#include "yack/math/algebra/crout.hpp"
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


        typedef meta_list<const equilibrium> ep_list;
        typedef ep_list::node_type           ep_node;

        class ep_group : public object, public ep_list
        {
        public:
            explicit ep_group() throw() : object(), ep_list(), next(0), prev(0) {}
            virtual ~ep_group() throw() {}

            bool linked_to(const equilibrium &eq, const imatrix &NuA) const throw()
            {
                assert(size>0);
                const readable<int> &lhs = NuA[*eq];
                //std::cerr << "testing " << eq.name << " @" << lhs << std::endl;
                for(const ep_node *en=head;en;en=en->next)
                {
                    const equilibrium   &me  = **en;
                    const readable<int> &rhs = NuA[*me];
                    //std::cerr << "against " << me.name << " @" << rhs;
                    if(linkedRows(lhs,rhs))
                    {
                        //std::cerr << " [+]" << std::endl;
                        return true;
                    }
                    //else std::cerr << " [-]" << std::endl;

                }
                return false;
            }

            ep_group *next;
            ep_group *prev;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ep_group);
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


        void reactor:: conservation(const xmlog &xml)
        {
            YACK_XMLSUB(xml, "Conservation");


            NuA.assign(Nu);
            vector<species *>     sdb(M,as_capacity); // TODO: necessary ?
            ep_list               edb;
            addrbook              adb;

            //------------------------------------------------------------------
            //
            //
            // Creating restricted NuA by filtering equilibria
            //
            //
            //------------------------------------------------------------------
            {

                //--------------------------------------------------------------
                //
                // Looping over species
                //
                //--------------------------------------------------------------
                for(const anode *an=working.head;an;an=an->next)
                {
                    const species &s = **an;
                    const size_t   j = *s;
                    const islot   &l = held_by[j];

                    if(verbose) corelib.pad(*xml<< s.name,s) << " : " << l << std::endl;
                    assert(s.rank==l.size);

                    bool    keep = true;
                    size_t  n    = 0;
                    size_t  p    = 0;

                    for(const enode *en=singles.head();en;en=en->next)
                    {
                        const equilibrium &eq = ***en;
                        const size_t       i  = *eq;
                        const feature      f  = eq.kind();

                        switch( __sign::of(Nu[i][j]) )
                        {
                            case __zero__:
                                continue;

                            case positive:
                                ++p;
                                break;

                            case negative:
                                ++n;
                                break;
                        }

                        YACK_XMLOG(xml,"|_[" << p << "+|" << n << "-]");

                        if(p>1||n>1)
                        {
                            // not a conservative equilibrium
                            keep = false;
                            YACK_XMLOG(xml,"|_shared");
                            notConserved(NuA,j);
                            goto DONE;
                        }

                        switch(f)
                        {
                            case undefined:
                                // bad!
                                throw exception("undefined equilibrium <%s>", eq.name() );

                            case part_only:
                            case join_only:
                                // not a conservative equilibrium
                                keep = false;
                                YACK_XMLOG(xml,"|_excluded by " << eq.name);
                                notConserved(NuA,j);
                                goto DONE;

                            case both_ways:
                                break;
                        }

                        assert(both_ways==f);
                        adb.ensure(&eq);
                    }

                DONE:
                    if(!keep) continue;
                    sdb << & coerce(s);
                }


                YACK_XMLOG(xml,"--> #species    = " << std::setw(6) <<  sdb.size()  << " / " << std::setw(6) << M);
                YACK_XMLOG(xml,"--> #equilibria = " << std::setw(6) << (*adb).size  << " / " << std::setw(6) << N);

                if(sdb.size() <= 0)
                {
                    return;
                }

                // retrieve equilibria
                for(addrbook::const_iterator it=adb.begin();it!=adb.end();++it)
                {
                    const void        *addr = *it; assert(NULL!=addr);
                    const equilibrium &eq   = *static_cast<const equilibrium *>(addr);
                    edb << & coerce(eq);
                }
            }



            std::cerr << "Nu  = " << Nu  << std::endl;
            std::cerr << "NuA = " << NuA << std::endl;


            singles.graphviz("eqs.dot",corelib);

            //------------------------------------------------------------------
            //
            //
            // Grouping filtered equilibria, using NuA connectivity
            //
            //
            //------------------------------------------------------------------
            {
                YACK_XMLSUB(xml, "CreatingGroups");
                ep_groups groups;

                //--------------------------------------------------------------
                //
                // loop over filtered eqs
                //
                //--------------------------------------------------------------
                while(edb.size)
                {
                    auto_ptr<ep_node>    en  = edb.pop_front();
                    const equilibrium   &eq  = **en;
                    YACK_XMLOG(xml, "--> <" << eq.name << "> ");
                    assert( both_ways == eq.kind() );
                    assert( iota::dot<int>::of(NuA[*eq],NuA[*eq])>0);

                    // look for a connex group
                    ep_group     *grp = NULL;
                    for(ep_group *g=groups.head;g;g=g->next)
                    {
                        if(g->linked_to(eq,NuA)) {
                            grp = g;
                            break;
                        }
                    }

                    // if no connex group was found, build a new one
                    if(!grp) groups.push_back( grp = new ep_group() );
                    assert(grp);

                    // record equilibrium in current group
                    grp->push_back( en.yield() );
                }

                YACK_XMLOG(xml, "--> |group| = " << groups.size);

                //--------------------------------------------------------------
                //
                // creating constraints
                //
                //--------------------------------------------------------------
                if(groups.size>0)
                {
                    YACK_XMLSUB(xml,"CreatingConstraints");
                    size_t igrp = 0;
                    for(const ep_group *grp=groups.head;grp;grp=grp->next)
                    {
                        YACK_XMLSUB(xml,"Constraint");

                        //------------------------------------------------------
                        // record all involved species from one group
                        //------------------------------------------------------
                        ++igrp;
                        if(verbose) *xml << "-- using group #" << std::setw(4) << igrp << std::endl;
                        const size_t rows = grp->size;
                        assert(rows>0);

                        adb.free();
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
                        const size_t   cols = (*adb).size;
                        vector<size_t> jcol(cols,as_capacity);
                        sdb.free();
                        for(addrbook::const_iterator it=adb.begin();it!=adb.end();++it)
                        {
                            const void    *addr = *it; assert(NULL!=addr);
                            const species &s    = *static_cast<const species *>(addr);
                            sdb  << & coerce(s);
                            jcol << *s;
                        }

                        if(verbose) {
                            *xml << "\t|rows| = " << rows << std::endl;
                            *xml << "\t|cols| = " << cols << ':';
                            for(size_t i=1;i<=cols;++i)
                            {
                                std::cerr << ' ' << sdb[i]->name;
                            }
                            std::cerr << std::endl;
                        }


                        //------------------------------------------------------
                        // check status
                        //------------------------------------------------------
                        if(rows>=cols)
                        {
                            YACK_XMLOG(xml, "\tno constraint");
                            continue;
                        }
                        const size_t cons = cols-rows;
                        YACK_XMLOG(xml, "\t|cons| = " << cons);

                        //------------------------------------------------------
                        // create constraints matrix
                        //------------------------------------------------------
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

                        
                        matrix<apq> Pt(P,transposed);
                        matrix<apq> P2(rows,rows);
                        iota::mmul(P2,P,Pt);
                        matrix<apq> iP2(P2);
                        crout<apq>  lu(rows);

                        std::cerr << "P    = " << P << std::endl;
                        std::cerr << "P2   = " << P2 << std::endl;
                        if(!lu.build(iP2)) throw exception("singular topology!!");
                        const apq  detP2 = lu.determinant(iP2); assert(0!=detP2);
                        std::cerr << "detP2 = " << detP2 << std::endl;
                        matrix<apq> adjP2(rows,rows);
                        lu.adjoint(adjP2,P2);
                        std::cerr << "adjP2 = " << adjP2 << std::endl;

                        matrix<apq> adjP3(rows,cols);
                        iota::mmul(adjP3,adjP2,P);
                        std::cerr << "adjP3 = " << adjP3 << std::endl;
                        matrix<apq> invP(cols,cols);
                        iota::mmul(invP,Pt,adjP3);
                        std::cerr << "invP  = " << invP << std::endl;
                        matrix<apq> Qvec(cols,cols);
                        for(size_t i=cols;i>0;--i)
                        {
                            for(size_t j=cols;j>0;--j)
                            {
                                if(i==j)
                                {
                                    Qvec[i][i] = detP2 - invP[i][i];
                                }
                                else
                                {
                                    Qvec[i][j] = - invP[j][i];
                                }
                            }
                        }
                        std::cerr << "Qvec  = " << Qvec << std::endl;
                    }

                }

            }


            exit(0);
            //"@eq1:-[a]+[b]:1"  "@eq2:-[b]+2[c]:1"  "@eq3:-[c]+[d]+[a]:1"
        }

    }

}

