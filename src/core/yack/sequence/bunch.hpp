//! \file
#ifndef YACK_BUNCH_INCLUDED
#define YACK_BUNCH_INCLUDED 1

#include "yack/sequence/cxx-array.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/object.hpp"

namespace yack
{

    template <typename T>
    class bunch
    {
    public:
        YACK_DECL_ARGS(T,type);
        typedef cxx_array<mutable_type> array_type;

        class entry : public object, public array_type
        {
        public:
            inline explicit entry(const size_t w) : array_type(w), next(0), prev(0) {}
            inline virtual ~entry() throw() {}

            entry *next;
            entry *prev;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(entry);
        };

        typedef cxx_list_of<entry> entries;



        inline explicit bunch(const size_t w) throw() :
        width(w)
        {
        }

        inline virtual ~bunch() throw() {}

        inline void reserve(size_t n)
        {
            while(n-- > 0) zpool.push_back( new entry(width) );
        }

        inline void free()
        {
            zpool.merge_front(alive);
        }

        const size_t width;



    private:
        YACK_DISABLE_COPY_AND_ASSIGN(bunch);
        entries alive; //!< in use
        entries zpool; //!< unsued
    };

}

#endif

