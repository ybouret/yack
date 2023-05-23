
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
        //! base class for zvector
        typedef cxx_array<apz,memory::dyadic> ztableau;

        //______________________________________________________________________
        //
        //
        //! vectors of apex integers
        //
        //______________________________________________________________________
        class zvector : public spot_object, public metrics, public ztableau
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //______________________________________________________________________
            explicit zvector(const size_t dims); //!< setup
            virtual ~zvector() noexcept;         //!< cleanup
            zvector(const zvector &);            //!< copy

            zvector *next;   //!< for list
            zvector *prev;   //!< for list
            size_t   ncoeff; //!< number of coefficients not equal to zero
            apn      weight; //!< |this|_1

            void       finalize(); //!< compute ncoeff and weight
            static int compare(const zvector *lhs,
                               const zvector *rhs) noexcept; //!< hybrid comparison

        private:
            YACK_DISABLE_ASSIGN(zvector);
        };
    }

}

#endif

