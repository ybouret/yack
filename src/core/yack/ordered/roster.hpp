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

    template <typename T, typename COMPARATOR>
    class roster : public container
    {
    public:
        YACK_DECL_ARGS(T,type);
        typedef solo_list<T>                  list_type;
        typedef typename list_type::node_type node_type;

        inline explicit roster() noexcept : container(), my()
        {
        }

        inline virtual ~roster() noexcept {}

        inline virtual const char *category()  const noexcept { return low_level::roster_category; }
        inline virtual size_t      size()      const noexcept { return my.size; }
        inline virtual size_t      capacity()  const noexcept { return my.size+my.ready(); }
        inline virtual size_t      available() const noexcept { return my.ready();}
        inline virtual void        free()            noexcept { my.clear(); }
        inline virtual void        release()         noexcept { my.clear(); my.cache->release(); }
        inline virtual void        reserve(size_t n)          { return my.cache->reserve(n); }


        inline void push(param_type args)
        {
            node_type  *node = my.new_node(args);
            const_type &lhs  = **node;
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(roster);
        list_type my;
    };

}

#endif

