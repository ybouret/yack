#include "yack/chem/reactor/cluster.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace chemical
    {


        const char * const cluster:: clid = "chemical::cluster";

        cluster:: ~cluster() throw()
        {
        }
        
        cluster:: cluster(const equilibrium &first) :
        object(),
        latch(),
        next(0),
        prev(0),
        alive( new alist()   ),
        group( new glist()   ),
        genus( new eq_tier() ),
        breed( new sp_tier() ),
        gvidx(0)
        {
            coerce( *group ) << first;
        }

        
        bool  cluster:: owns(const equilibrium &eq) const throw()
        {
            for(const gnode *gn=(**group).head;gn;gn=gn->next)
            {
                if( &(gn->host) == &eq ) return true;
            }
            return false;
        }

        void cluster:: grow(const equilibrium &eq)
        {
            if( latched() ) throw imported::exception(clid,"grow(already latched)");
            assert(!owns(eq));
            coerce( *group ) << eq;
        }

        bool cluster:: linked_with(const equilibrium  &eq,
                                   const matrix<bool> &related) const throw()
        {
            assert( !owns(eq) );
            const readable<bool> &chk = related[ *eq ];
            for(const gnode *gn=(**group).head;gn;gn=gn->next)
            {
                if( chk[ *(gn->host) ] ) return true;
            }
            return false;
        }

        static inline void show_gnode(const gnode &g, std::ostream &os)
        {
            os << '<' << g.host.name << '>' << '@' << *g;
        }

        std::ostream & cluster:: display(std::ostream &os) const
        {
            os << '[';
            const gnode *gn = (**group).head;
            if(gn)
            {
                show_gnode(*gn,os);
                for(gn=gn->next;gn;gn=gn->next)
                    show_gnode(*gn,  os << ", ");
            }
            return os << ']';
        }

        std::ostream & operator<<( std::ostream &os, const cluster &self)
        {
            return self.display(os);
        }


        static inline int compare_sp(const species &lhs, const species &rhs) throw()
        {
            return comparison::increasing(*lhs,*rhs);
        }


        static inline void populate(sp_repo &slist, const addrbook &tribe)
        {
            for(addrbook::const_iterator it=tribe.begin();it!=tribe.end();++it)
                slist << *static_cast<const species *>(*it);
            slist.sort_with(compare_sp);
        }

        void cluster:: collect_alive()
        {

            sp_repo slist;

            //------------------------------------------------------------------
            // collect all species
            //------------------------------------------------------------------
            {
                addrbook tribe;
                for(const gnode *gn = (*group)->head;gn;gn=gn->next)
                    gn->host.update(tribe);
                populate(slist,tribe);
            }

            //------------------------------------------------------------------
            // transfert to active list
            //------------------------------------------------------------------
            alist &my = coerce( *alive );
            for(const sp_node *sn=slist.head;sn;sn=sn->next)
                my << ***sn;
        }

        static inline
        bool are_all_unbounded(const actors   &players,
                               const addrbook &tribe) throw()
        {
            for(const actor *a=players->head;a;a=a->next)
            {
                const species &s = **a;
                if( !tribe.search(&s) ) return false;
            }
            return true;
        }

        static inline
        bool is_special(const equilibrium &eq, const addrbook &tribe) throw()
        {
            return are_all_unbounded(eq.reac,tribe) || are_all_unbounded(eq.prod,tribe);
        }



        void cluster:: collect_genus()
        {

            eq_group  remaining;

            eq_group & roaming = coerce( *(genus->roaming) );
            eq_group & bounded = coerce( *(genus->bounded) );
            eq_group & special = coerce( *(genus->special) );

            //------------------------------------------------------------------
            // first pass: collect obvious roaming eqs and species
            //------------------------------------------------------------------
            addrbook   tribe;
            for(const gnode *gn = (*group)->head; gn; gn=gn->next )
            {
                const equilibrium &eq = gn->host;
                if(eq.reac->size<=0 || eq.prod->size<=0)
                {
                    roaming << *gn;
                    eq.update(tribe);
                }
                else
                {
                    remaining  << *gn;
                }
            }

            //------------------------------------------------------------------
            // second pass: processing remaining to detect more unbounded
            //------------------------------------------------------------------
            {
            SECOND_PASS:
                for(const eq_gnode *node=remaining.head;node;node=node->next)
                {
                    const equilibrium &eq = (***node).host;
                    if(is_special(eq,tribe))
                    {
                        if(eq.update(tribe))
                        {
                            // updated list of species
                            goto SECOND_PASS;
                        }
                    }
                }
            }


            //------------------------------------------------------------------
            // third pass: dispatch remaining
            //------------------------------------------------------------------
            while( remaining.size )
            {
                eq_gnode          *gn = remaining.pop_front();
                const equilibrium &eq = (***gn).host;
                if(is_special(eq,tribe))
                    special.push_back(gn);
                else
                    bounded.push_back(gn);
            }

            //------------------------------------------------------------------
            // finalize: dispatch species
            //------------------------------------------------------------------
            for(const anode *sn = (*alive)->head; sn; sn=sn->next )
            {
                const species &s = sn->host;
                if( tribe.search(&s) )
                {
                    coerce( *(breed->unbounded) ) << *sn;
                }
                else
                {
                    coerce( *(breed->conserved) ) << *sn;
                }
            }


        }

        void cluster:: compile(const xmlog &xml, const unsigned igv)
        {

            YACK_XMLSUB(xml,"cluster::compile");
            if( latched() ) throw imported::exception(clid,"compile(already latched)");
            YACK_XMLOG(xml,"-- group     : " << *this);

            collect_alive();
            YACK_XMLOG(xml,"-- alive     : " << **alive);

            collect_genus();
            YACK_XMLOG(xml,"-- bounded   : " << genus->bounded);
            YACK_XMLOG(xml,"-- roaming   : " << genus->roaming);
            YACK_XMLOG(xml,"-- special   : " << genus->special);

            YACK_XMLOG(xml,"-- conserved : " << breed->conserved);
            YACK_XMLOG(xml,"-- unbounded : " << breed->unbounded);


            coerce(gvidx) = igv;
            lock();
        }


        static inline void sp_viz(ios::ostream  &fp,
                                  const species &s,
                                  const char    *attr)
        {
            s.logo(fp) << '[';
            s.add_label(fp,s.name());
            fp << attr;
            s.end(fp << ']');
        }

        static inline void eq_viz(ios::ostream &fp,
                                  const equilibrium &eq,
                                  const char *attr)
        {

            eq.logo(fp) << '[';
            eq.add_label(fp,eq.name());
            fp << attr;
            eq.end(fp << ']');

            for(const actor *a = eq.reac->head; a;a=a->next)
            {
                const species &s = **a;
                const unsigned nu = a->nu;
                s.link(fp,&eq);
                if(nu>1) fp("[label=\"%u\"]",nu);
                s.end(fp);
            }

            for(const actor *a = eq.prod->head; a;a=a->next)
            {
                const species &s = **a;
                const unsigned nu = a->nu;
                eq.link(fp,&s);
                if(nu>1) fp("[label=\"%u\"]",nu);
                eq.end(fp);
            }
        }

        void cluster:: viz(ios::ostream &fp) const
        {
            fp << "subgraph cluster_";
            fp("%u",gvidx);
            fp << " {\n";

            // write all species
            for(const sp_gnode *sn=breed->conserved->head;sn;sn=sn->next)
            {
                const species &s = (***sn).host;
                sp_viz(fp,s, ", shape=rectangle,style=filled");
            }

            for(const sp_gnode *sn=breed->unbounded->head;sn;sn=sn->next)
            {
                const species &s = (***sn).host;
                sp_viz(fp,s, ", shape=rectangle");
            }

            
            // write all equilibria
            for(const eq_gnode *en=genus->bounded->head;en;en=en->next)
            {
                eq_viz(fp,(***en).host,",shape=oval,style=bold");
            }

            for(const eq_gnode *en=genus->roaming->head;en;en=en->next)
            {
                eq_viz(fp,(***en).host,",shape=octagon");
            }

            for(const eq_gnode *en=genus->special->head;en;en=en->next)
            {
                eq_viz(fp,(***en).host,",shape=octagon,style=dashed");
            }


            fp << "}\n";
        }


    }
}

namespace yack
{
    namespace chemical
    {
        clusters:: clusters() throw() : object(), cxx_list_of<cluster>()
        {
        }
        
        clusters:: ~clusters() throw()
        {
        }
        
    }
    
}
