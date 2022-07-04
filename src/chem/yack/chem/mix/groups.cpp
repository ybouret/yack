#include "yack/chem/mix/groups.hpp"
#include "yack/data/list/sort.hpp"

namespace yack
{
    namespace chemical
    {
        groups:: ~groups() throw() {}

        groups:: groups() throw() : group::list()
        {
        }

        void groups:: sort()
        {
            for(group *g=head;g;g=g->next)
            {
                g->sort();
            }
            merge_list_of<group>::sort(*this,group::compare);
        }

        bool groups:: contains(const equilibrium &eq) const throw()
        {
            for(const group *g=head;g;g=g->next)
            {
                if(g->contains(eq)) return true;
            }
            return false;
        }


        bool groups:: contains(const equilibrium &x, const equilibrium &y) const throw()
        {
            for(const group *g=head;g;g=g->next)
            {
                if(g->contains(x,y)) return true;
            }
            return false;
        }



        std::ostream & operator<<(std::ostream &os, const groups &G)
        {
            if(G.size>0)
            {
                os << "<groups count=\"" << G.size << "\">" << std::endl;
                for(const group *g=G.head;g;g=g->next)
                {
                    os << "  " << *g << std::endl;
                }
            }
            os << "<groups/>";
            return os;
        }

    }


}

#include "yack/sequence/cxx-array.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace chemical
    {
        groups:: groups(const equilibria &lattice, const build_clusters_ &) : group::list()
        {
            //------------------------------------------------------------------
            //
            // build independent attached groups
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("building attached partitions");
            const groups attached(lattice,groups::build_attached);
            YACK_CHEM_PRINTLN("attached=" << attached);

            //------------------------------------------------------------------
            //
            // create global detached matrix
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("building detached matrix");
            const matrix<bool> detached;
            lattice.build( coerce(detached) );
            if(entity::verbose)
            {
                lattice(std::cerr << "detached=",detached,"");
            }

            //------------------------------------------------------------------
            //
            // full build
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("building partitions");
            {
                //--------------------------------------------------------------
                // create partition from attached groups
                //--------------------------------------------------------------
                const size_t      dims = attached.size;
                cxx_array<groups> part(dims);
                {
                    size_t i=0;
                    for(const group *g=attached.head;g;g=g->next)
                    {
                        groups p(*g,detached);
                        part[++i].swap_with(p);
                    }
                }

                //--------------------------------------------------------------
                // weave partitions
                //--------------------------------------------------------------
                YACK_CHEM_PRINTLN("weaving partitions");
                groups woven(part);
                swap_with(woven);
            }

            //------------------------------------------------------------------
            //
            // full check
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("checking " << size << " groups");
            for(const enode *x=lattice.head();x;x=x->next)
            {
                const equilibrium    &ex = ***x; if(!contains(ex)) throw exception("missing '%s' in groups",ex.name());
                const readable<bool> &ok = detached[*ex];

                for(const enode *y=x->next;y;y=y->next)
                {
                    const equilibrium &ey = ***y;
                    if(ok[*ey])
                    {
                        if(!contains(ex,ey)) throw exception("missing detached '%s/%s' in groups", ex.name(), ey.name());
                    }
                }
            }
        }

        groups * groups:: create_from(const equilibria &lattice)
        {
            return new groups(lattice,build_clusters);
        }
    }
}
