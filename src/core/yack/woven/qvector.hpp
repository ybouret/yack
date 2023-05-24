//! \file

#ifndef YACK_WOVEN_QVECTOR_INCLUDED
#define YACK_WOVEN_QVECTOR_INCLUDED 1

#include "yack/woven/q-array.hpp"
#include "yack/container/writable.hpp"
#include "yack/spot-object.hpp"

namespace yack
{
    namespace woven
    {
        //______________________________________________________________________
        //
        //
        //! univocal vector of integers (as rationals)
        //
        //______________________________________________________________________
        class qvector : public object, public q_array
        {
        public:
            //__________________________________________________________________
            //
            //  C++
            //__________________________________________________________________
            virtual ~qvector() noexcept; //!< cleanup
            qvector(const qvector &);    //!< copy

            //! setup from compatible user's data
            template <typename T> inline
            qvector(const readable<T> &arr) : object(), q_array(arr.size()), norm2(*znrm2)
            {
                for(size_t i=dimensions;i>0;--i)
                        coord[i] = arr[i];
                finalize();
            }

            //__________________________________________________________________
            //
            //  methods
            //__________________________________________________________________

            //! keep rational orthogonal component of source from *this
            /**
             \param result rational orthogonal reminder
             \param source a compatible test vector
             */
            template <typename SOURCE>
            bool make_ortho(writable<apq> &result, SOURCE &source) const {
                apq fac;
                for(size_t i=dimensions;i>0;--i) fac +=  coord[i] * source[i];
                fac /= norm2;
                for(size_t i=dimensions;i>0;--i) result[i] = source[i] - fac * coord[i];
                return check_not_null(result);
            }

            //__________________________________________________________________
            //
            //  members
            //__________________________________________________________________
            const apz &norm2; //!< reference to internal |coord|^2




        private:
            YACK_DISABLE_ASSIGN(qvector);

        };



    }

}

#endif

