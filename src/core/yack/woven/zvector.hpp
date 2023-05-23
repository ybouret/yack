
//! \file

#ifndef YACK_WOVEN_ZVECTOR_INCLUDED
#define YACK_WOVEN_ZVECTOR_INCLUDED 1

#include "yack/woven/qvector.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/memory/allocator/dyadic.hpp"

namespace yack
{
    namespace woven
    {
        typedef cxx_array<apz,memory::dyadic> ztableau;

        class zvector : public spot_object, public metrics, public ztableau
        {
        public:
            explicit zvector(const size_t dims);
            virtual ~zvector() noexcept;
            zvector(const zvector &);

            zvector *next;
            zvector *prev;
            size_t   ncoeff;
            apn      weight;

            void       finalize();
            static int compare(const zvector *lhs, const zvector *rhs) noexcept;

        private:
            YACK_DISABLE_ASSIGN(zvector);
        };
    }

}

#endif

