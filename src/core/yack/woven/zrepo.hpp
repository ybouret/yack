
//! \file

#ifndef YACK_WOVEN_QREPOSITORY_INCLUDED
#define YACK_WOVEN_QREPOSITORY_INCLUDED 1

#include "yack/woven/qvector.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/memory/allocator/dyadic.hpp"

namespace yack
{
    namespace woven
    {

        typedef cxx_array<apz,memory::dyadic> ztableau;

        class zvector : public object, public metrics, public ztableau
        {
        public:
            explicit zvector(const size_t dims);
            virtual ~zvector() noexcept;
            zvector(const zvector &);

            zvector *next;
            zvector *prev;
        private:
            YACK_DISABLE_ASSIGN(zvector);
        };

        typedef cxx_list_of<zvector> zvectors;

        class zrepo : public metrics, public zvectors
        {
        public:
            explicit zrepo(const size_t dims);
            virtual ~zrepo() noexcept;
            zrepo(const zrepo &);

            void ensure(const qvector          &lhs);
            void ensure(const list_of<qvector> &lhs);

        private:
            YACK_DISABLE_ASSIGN(zrepo);
        };

    }

}

#endif

