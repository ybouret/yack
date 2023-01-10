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
        large_object(),
        latch(),
        next(0),
        prev(0),
        alive( new alist()   ),
        group( new glist()   ),
        breed( new sp_tier() ),
        genus( new eq_tier() ),
        sdict( new udict("species")     ),
        edict( new udict("equilibrium") ),
        canon( new claws() ),
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


#if 0
        static const char build_dict_fn[] = "cluster dictionary";

        template <typename NODE>
        static inline
        void build_dict(udict      &dict,
                        const NODE *node,
                        const char *kind)
        {
            assert(kind);
            for(;node;node=node->next)
            {
                const entity &e = node->host;
                const size_t  i = *e;
                const size_t  I = **node;
                if(!dict.insert(i,I)) {
                    throw imported::exception(build_dict_fn,"multiple %s index",kind);
                }
            }
        }
#endif

        void cluster:: compile(const xmlog &xml, const unsigned igv)
        {

            YACK_XMLSUB(xml,"cluster::compile");
            if( latched() ) throw imported::exception(clid,"compile(already latched)");
            YACK_XMLOG(xml,"-- group     : " << *this);

            collect_alive();
            create_system();

            //build_dict( coerce(*edict), (*group)->head, "equilibrium");
            //build_dict( coerce(*sdict), (*alive)->head, "species");

            coerce( *sdict ).record( (*alive)->head );
            coerce( *edict ).record( (*group)->head );



            YACK_XMLOG(xml,"-- alive     : " << **alive);
            YACK_XMLOG(xml,"-- bounded   : " << genus->bounded);
            YACK_XMLOG(xml,"-- roaming   : " << genus->roaming);
            YACK_XMLOG(xml,"-- special   : " << genus->special);
            YACK_XMLOG(xml,"-- conserved : " << breed->conserved);
            YACK_XMLOG(xml,"-- unbounded : " << breed->unbounded);

            coerce(gvidx) = igv;

            conservations(xml);

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
