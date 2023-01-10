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
        act( new alist() ),
        grp( new glist() )
        {
            coerce( *grp ) << first;
        }


        const list_of<gnode> * cluster:: operator->() const throw()
        {
            return & (**grp);
        }

        const list_of<gnode> & cluster:: operator*() const throw()
        {
            return (**grp);
        }

        bool  cluster:: owns(const equilibrium &eq) const throw()
        {
            for(const gnode *gn=(**grp).head;gn;gn=gn->next)
            {
                if( &(gn->host) == &eq ) return true;
            }
            return false;
        }

        void cluster:: grow(const equilibrium &eq)
        {
            if( latched() ) throw imported::exception(clid,"grow(already latched)");
            assert(!owns(eq));
            coerce( *grp ) << eq;
        }

        bool cluster:: linked_with(const equilibrium  &eq,
                                   const matrix<bool> &related) const throw()
        {
            assert( !owns(eq) );
            const readable<bool> &chk = related[ *eq ];
            for(const gnode *gn=(**grp).head;gn;gn=gn->next)
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
            const gnode *gn = (**grp).head;
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

        void cluster:: collect_active()
        {

            sp_repo slist;

            //------------------------------------------------------------------
            // collect all species
            //------------------------------------------------------------------
            {
                addrbook tribe;
                for(const gnode *gn = (*grp)->head;gn;gn=gn->next)
                    gn->host.update(tribe);


                for(addrbook::const_iterator it=tribe.begin();it!=tribe.end();++it)
                    slist << *static_cast<const species *>(*it);
            }

            //------------------------------------------------------------------
            // ensure sorted by index
            //------------------------------------------------------------------
            slist.sort_with(compare_sp);

            //------------------------------------------------------------------
            // transfert to active list
            //------------------------------------------------------------------
            alist &my = coerce( *act );
            for(const sp_node *sn=slist.head;sn;sn=sn->next)
            {
                my << ***sn;
            }
        }

        void cluster:: compile(const xmlog &xml)
        {
            YACK_XMLSUB(xml,"cluster::compile");
            if( latched() ) throw imported::exception(clid,"compile(already latched)");
            YACK_XMLOG(xml,"-- " << *this);

            collect_active();
            YACK_XMLOG(xml,"-- " << **act);


            lock();
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
