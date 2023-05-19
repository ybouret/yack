//! \file

#ifndef YACK_WOVEN_QVECTOR_INCLUDED
#define YACK_WOVEN_QVECTOR_INCLUDED 1

#include "yack/woven/q-array.hpp"
#include "yack/apex/flak.hpp"

namespace yack
{
    namespace woven
    {
        
        class qvector : public object, public q_array
        {
        public:
            virtual ~qvector() noexcept;
            qvector(const qvector &);

            template <typename T> inline
            qvector(const readable<T> &arr) :
            q_array(arr.size()),
            norm2(*znrm2)
            {
                for(size_t i=dimensions;i>0;--i)
                        coord[i] = arr[i];
                finalize();
            }

            const apz &norm2;

            //! keep rational orthogonal component of source from *this
            template <typename SOURCE>
            bool ortho(writable<apq> &result, SOURCE &source) {
                apex::flak::orthogonal_difference(result, source, *this);
                return check_not_null(result);
            }


        private:
            YACK_DISABLE_ASSIGN(qvector);

        };

    }

}

#endif

