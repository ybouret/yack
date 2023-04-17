//! \file

#ifndef YACK_ORDERED_ROSTER_INCLUDED
#define YACK_ORDERED_ROSTER_INCLUDED 1

#include "yack/data/dinky/solo-list.hpp"
#include "yack/container.hpp"

namespace yack
{

    namespace low_level
    {
        extern const char roster_category[];
    }

    //__________________________________________________________________________
    //
    //
    //! ordered list with sign_type COMPARATOR(lhs,rhs)
    //
    //__________________________________________________________________________
    template <typename T, typename COMPARATOR>
    class roster : public container
    {
    public:
        //______________________________________________________________________
        //
        // definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);
        typedef solo_list<T>                  list_type;
        typedef typename list_type::node_type node_type;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit roster() noexcept : container(), my()
        {
        }

        inline virtual ~roster() noexcept {}

        //______________________________________________________________________
        //
        // virtual interface
        //______________________________________________________________________
        inline virtual const char *category()  const noexcept { return low_level::roster_category; }
        inline virtual size_t      size()      const noexcept { return my.size; }
        inline virtual size_t      capacity()  const noexcept { return my.size+my.ready(); }
        inline virtual size_t      available() const noexcept { return my.ready();}
        inline virtual void        free()            noexcept { my.clear(); }
        inline virtual void        release()         noexcept { my.clear(); my.cache->release(); }
        inline virtual void        reserve(size_t n)          { return my.cache->reserve(n); }

        //______________________________________________________________________
        //
        // mehtods
        //______________________________________________________________________
        inline void push(param_type args)
        {
            node_type  *node = my.new_node(args);
            try {
                switch(my.size)
                {
                    case 0: my.push_back(node); break;
                    case 1: push_second(node);  break;
                    case 2: push_higher(node);  break;
                }
                assert(is_valid());
            }
            catch(...) { my.zombify(node); }
        }

        inline friend std::ostream & operator<<(std::ostream &os, const roster &self)
        {
            os << self.my;
            return os;
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(roster);
        list_type          my;
        mutable COMPARATOR cm;

        inline bool is_valid() const {
            const node_type *curr = my.head;
            while(curr && curr->next)
            {
                const node_type *next = curr->next;
                switch(cm(**curr,**next))
                {
                    case __zero__:
                    case negative: break;
                    case positive: return false;
                }

                curr = next;
            }
            return true;
        }

        inline void push_second(node_type *node) noexcept
        {
            switch( cm(**node,**my.head) )
            {
                case negative: my.push_front(node); break;
                case __zero__:
                case positive: my.push_back(node);  break;
            }
        }

        inline node_type *preceding(const node_type *node)
        {
            assert(node);
            assert(my.size>=2);

            //------------------------------------------------------------------
            // test against lower bound
            //------------------------------------------------------------------
            node_type  *lower = my.head;
            const_type &value = **node;
            switch( cm(value,**lower) )
            {
                case negative: return NULL;
                case __zero__: return lower;
                case positive: break;
            }

            //------------------------------------------------------------------
            // test against upper bound
            //------------------------------------------------------------------
            node_type *upper = my.tail; assert(upper!=lower);
            switch( cm(**upper,value) )
            {
                case __zero__:
                case negative: return upper;
                default:       break;
            }

        }

        inline void push_higher(node_type *node)
        {
            assert(node);
            assert(my.size>=2);
        }
    };

}

#endif

