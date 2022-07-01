
#include "yack/chem/mix/group.hpp"
#include "yack/exception.hpp"
#include "yack/data/list/sort.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    namespace chemical
    {

        group:: ~group() throw()
        {
        }

        group:: group() throw() : next(0), prev(0)
        {

        }

        group * group:: new_from(const equilibrium &eq)
        {
            auto_ptr<group> g = new group();
            (*g) << &eq;
            return g.yield();
        }


        int group:: compare_(const gnode *lhs,
                             const gnode *rhs)
        {
            assert(lhs);
            assert(rhs);

            const equilibrium &L = **lhs;
            const equilibrium &R = **rhs;
            const size_t       l = *L;
            const size_t       r = *R;
            if(l<r)
            {
                return -1;
            }
            else
            {
                if(r<l)
                    return 1;
                else
                {
                    throw exception("in chemical::group: same index for '%s' and '%s'", L.name(), R.name() );
                    return 0;
                }
            }
        }


        int group:: compare(const group *lhs, const group *rhs)
        {
            assert(lhs);
            assert(rhs);
            assert(lhs->is_valid());
            assert(rhs->is_valid());

            size_t       nl = lhs->size;
            const size_t nr = rhs->size;
            if(nl<nr)
            {
                return -1;
            }
            else
            {
                if(nr<nl)
                {
                    return 1;
                }
                else
                {
                    const gnode *L = lhs->head;
                    const gnode *R = rhs->head;
                    while(nl-- > 0 )
                    {
                        const size_t l = ***L;
                        const size_t r = ***R;
                        if(l<r)
                        {
                            return -1;
                        }
                        else
                        {
                            if(r<l)
                            {
                                return 1;
                            }
                            else
                            {
                                // take next
                            }
                        }
                        L = L->next;
                        R = R->next;
                    }
                    throw exception("found same chemical::groups");
                    return 0;
                }

            }

        }


        void group:: sort()
        {
            merge_list_of<gnode>::sort(*this,compare_);
            assert(is_valid());
        }

        bool group:: is_valid() const throw()
        {
            const gnode *node = head;
            while(node)
            {
                const gnode *next = node->next;
                if(!next) break;
                if( (***next) <= (***node) ) return false;
                node = next;
            }
            return true;
        }


        bool group:: is_ortho() const throw()
        {
            const gnode *node = head;
            while(node)
            {
                const gnode *next = node->next;
                if(!next) break;
                const equilibrium &lhs = **node;
                const equilibrium &rhs = **next;
                if( lhs.attached(rhs) )
                {
                    std::cerr << "attached " << lhs.name << "," << rhs.name << std::endl;
                    return false;
                }
                node = next;
            }
            return true;
        }


        std::ostream & operator<<(std::ostream &os, const group &g)
        {
            os << '{';
            const gnode *node = g.head;
            if(node)
            {
                os << (**node).name;
                for(node=node->next;node;node=node->next)
                {
                    os << ',' << (**node).name;
                }
            }
            os << '}';
            return os;
        }

        bool group:: contains(const equilibrium &eq) const throw()
        {
            for(const gnode *node=head;node;node=node->next)
            {
                if( & **node == &eq ) return true;
            }
            return false;
        }


        bool group:: contains(const group &g) const throw()
        {
            for(const gnode *guest = g.head; guest; guest=guest->next)
            {
                if( ! contains(**guest) ) return false;
            }
            return true;
        }

        bool group:: attached(const equilibrium &eq) const throw()
        {
            for(const gnode *node=head;node;node=node->next)
            {
                if( eq.attached(**node) ) return true;
            }
            return false;
        }

        bool group:: detached(const equilibrium &eq) const throw()
        {
            for(const gnode *node=head;node;node=node->next)
            {
                if( !eq.detached(**node)) return false;
            }
            return true;
        }

    }

}
