#include "yack/chem/vat/cluster.hpp"
#include "yack/math/algebra/ortho-family.hpp"
#include "yack/system/imported.hpp"
#include "yack/woven/qcompress.hpp"
#include "yack/woven/qbuilder.hpp"

namespace yack
{
    namespace Chemical
    {

        void Cluster:: discoverLaws(const xmlog &xml)
        {
            const size_t nc = lib.size;
            if(nc<=1) return;
            YACK_XMLSUB(xml,"Cluster::discoverLaws");
            woven::zrepo     repo(nc);

            //------------------------------------------------------------------
            //
            // using WOVEn to build possible laws
            //
            //------------------------------------------------------------------
            {
                matrix<int>  Q(nc,nc);
                if(!math::ortho_family::build(Q,Nu,true))
                    throw imported::exception(CLID,"singular topology!!");

                matrix<int>  P;
                const size_t rank = woven::qcompress::build(P,Q);
                if(rank<=0)
                    throw imported::exception(CLID,"unexpected null rank");

                woven::qbuilder  work(nc);
                work(repo,P,rank,true);
            }

            //------------------------------------------------------------------
            //
            // process repo
            //
            //------------------------------------------------------------------
            typedef core_repo<const woven::zvector> zvecs;
            zvecs good;
            for(const woven::zvector *v=repo.head;v;v=v->next)
            {
                size_t nCoef = 0;
                bool   isPos = true;
                for(size_t i=v->size();i>0;--i)
                {
                    switch( (*v)[i].s )
                    {
                        case __zero__:          continue;
                        case positive: ++nCoef; continue;
                        case negative: isPos = false;
                            break;
                    }
                    break;
                }
                if(!isPos||nCoef<=1)  continue;
                good << *v;
            }

            if(good.size<=0)
            {
                YACK_XMLOG(xml,"no possible conservation");
                return;
            }

            //------------------------------------------------------------------
            //
            // create laws
            //
            //------------------------------------------------------------------
            const size_t qn = good.size;
            coerce(Qmat).make(qn,nc);
            {
                size_t i = 1;
                for(const zvecs::node_type *node=good.head;node;node=node->next,++i)
                {
                    writable<unsigned>  &u = coerce(Qmat)[i];
                    const readable<apz> &z = (***node);
                    for(size_t j=nc;j>0;--j)
                    {
                        u[j] = z[j].cast_to<unsigned>("law coefficient");
                    }

                    ConservationLaw &claw = * coerce(laws).push_back( new ConservationLaw() );
                    for(const SpNode *sn=lib.head;sn;sn=sn->next)
                    {
                        const Species &sp = ***sn;
                        const size_t   sj = sp.indx[SubLevel];
                        const unsigned nu = u[sj];
                        if(nu>0)
                        {
                            claw.push_back( new Actor(nu,sp) );
                        }
                    }

                }
            }
            coerce(Qrnk) = apex::flak::rank(Qmat);
            coerce(laws).finalize();
            YACK_XMLOG(xml, " Qmat = " << Qmat);
            YACK_XMLOG(xml, " Qrnk = " << Qrnk);
            YACK_XMLOG(xml, " laws = " << laws);

            
            assembleActs(xml);

        }


        void Cluster:: findOutRoles(const xmlog &xml)
        {
            YACK_XMLSUB(xml,"Cluster::findOutRoles");

            addrbook &cdb = coerce(conservedDB);
            addrbook &udb = coerce(unboundedDB);

            //------------------------------------------------------------------
            //
            // register all species as unbounded
            //
            //------------------------------------------------------------------
            for(const SpNode *node=lib.head;node;node=node->next)
            {
                const Species &sp = ***node;
                udb.ensure(&sp);
            }

            //------------------------------------------------------------------
            //
            // check output
            //
            //------------------------------------------------------------------
            for(const ConservationLaw *law=laws.head;law;law=law->next)
            {
                for(const Actor *ac=law->head;ac;ac=ac->next)
                {
                    const Species &sp = **ac;
                    udb.revoke(&sp);
                    cdb.ensure(&sp);
                }
            }

            //------------------------------------------------------------------
            //
            // build lists
            //
            //------------------------------------------------------------------
            {
                SpList &usl = coerce(unbounded);
                for(addrbook::const_iterator it=udb.begin();it!=udb.end();++it)
                {
                    usl << *static_cast<const Species*>( *it );
                }
            }

            {
                SpList &csl = coerce(conserved);
                for(addrbook::const_iterator it=cdb.begin();it!=cdb.end();++it)
                {
                    csl << *static_cast<const Species*>( *it );
                }
            }

            YACK_XMLOG(xml,"conserved : " << conserved);
            YACK_XMLOG(xml,"unbounded : " << unbounded);

            assert(lib.size==conserved.size+unbounded.size);

            //------------------------------------------------------------------
            //
            // sorting out eqs
            //
            //------------------------------------------------------------------
            for(const EqNode *node=head;node;node=node->next)
            {
                const Equilibrium &eq = ***node;
                const char        *id = NULL;
                if(eq.reac.size<=0)
                {
                    assert(eq.prod.size>0);
                    id = "prodOnly";
                    coerce(prodOnly) << eq;
                    goto END;
                }

                if(eq.prod.size<=0)
                {
                    assert(eq.reac.size>0);
                    id = "reacOnly";
                    coerce(reacOnly) << eq;
                    goto END;
                }
                


                assert(eq.reac.size>0);
                assert(eq.prod.size>0);

                continue;

            END:
                YACK_XMLOG(xml, " (" << id << ") " << eq.name);
            }


        }

        static inline void tryReduceActs(Acts &target)
        {
            Acts store;
            while(target.size)
            {
                Act *rhs = target.pop_front();
                for(Act *lhs = store.head; lhs; lhs=lhs->next)
                {
                    if(lhs->linkedTo(*rhs))
                    {
                        lhs->merge_back(*rhs);
                        delete rhs;
                        rhs = 0;
                        break;
                    }
                }

                if(rhs)
                    store.push_back(rhs);

            }
            target.swap_with(store);
        }

        void Cluster:: assembleActs(const xmlog &xml)
        {
            YACK_XMLSUB(xml,"Cluster::assembleActs");
            {
                Acts &target = coerce(acts);
                for(const ConservationLaw *node=laws.head;node;node=node->next)
                {
                    bool                   use = false;
                    const ConservationLaw &law = *node;
                    for(Act *canon=target.head;canon;canon=canon->next)
                    {
                        if(canon->endorses(law))
                        {
                            use = true;
                            *canon << law;
                            break;
                        }
                    }

                    if(use)
                    {
                        tryReduceActs(target);
                    }
                    else
                    {
                        target.push_back( new Act(law) );
                    }
                }
            }
            YACK_XMLOG(xml, "#acts=" << acts.size);

            for(Act *c=acts.head;c;c=c->next)
            {
                YACK_XMLOG(xml, *c);
                c->finalize();
            }


        }


    }

}


