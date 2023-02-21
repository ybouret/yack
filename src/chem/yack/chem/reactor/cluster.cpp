#include "yack/chem/reactor/cluster.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace chemical
    {


        const char * const cluster:: clid = "chemical::cluster";

        cluster:: ~cluster() noexcept
        {
        }
        
        cluster:: cluster(const equilibrium   &first,
                          const booleans::ptr &flags) :
        large_object(),
        latch(),
        next(0),
        prev(0),
        alive( new alist()              ),
        fixed( flags                    ),
        group( new glist()              ),
        single( new eq_repo()           ),
        hybrid( new eq_repo()           ),
        primary( new scene()            ),
        replica( new scene()            ),
        sDict( new udict("species")     ),
        eDict( new udict("equilibrium") ),
        canon( new claws()              ),
        claim( new clots()              ),
        army(  new squads()             ),
        wing(  new squads()             ),
        cross( new ledger()             ),
        gvidx(0),
        omega(0)
        {
            coerce( *group  ) << first;
            coerce( *single ) << first;
        }

        
        bool  cluster:: owns(const equilibrium &eq) const noexcept
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
            coerce( *group  ) << eq;
            coerce( *single ) << eq;
        }

        bool cluster:: linked_with(const equilibrium  &eq,
                                   const matrix<bool> &related) const noexcept
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





        void cluster:: compile(const xmlog            &xml,
                               const matrix<int>      &Nu,
                               const readable<double> &K,
                               const library          &lib,
                               const equilibria       &eqs,
                               equilibria             &all,
                               const unsigned          igv)
        {

            YACK_XMLSUB(xml,"cluster::compile");
            if( latched() ) throw imported::exception(clid,"compile(already latched)");

            {
                YACK_XMLSUB(xml,"cluster::primary");
                YACK_XMLOG(xml,"-- group     : " << *this);

                //--------------------------------------------------------------
                // preparing active species
                //--------------------------------------------------------------
                collect_alive();

                //--------------------------------------------------------------
                // initial breed/genus
                //--------------------------------------------------------------
                ignite_system();

                //--------------------------------------------------------------
                // create dictionaries
                //--------------------------------------------------------------
                coerce( *sDict ).record( (*alive)->head );
                coerce( *eDict ).record( (*group)->head );


                YACK_XMLOG(xml,"-- alive     : " << **alive);

                primary->display(xml, "primary");

            }
            coerce(gvidx) = igv;
            coerce(omega) = gvidx+1;

            // build conservation laws
            conservations(xml);

            // build manifold
            make_manifold(xml,Nu,K,lib,eqs,all);

            // consolidate from full group
            consolidating(xml);
            
            lock();
        }





    }
}

namespace yack
{
    namespace chemical
    {
        clusters:: clusters() noexcept : object(), cxx_list_of<cluster>()
        {
        }
        
        clusters:: ~clusters() noexcept
        {
        }
        
    }
    
}
