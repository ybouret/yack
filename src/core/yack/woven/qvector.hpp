//! \file

#ifndef YACK_WOVEN_QVECTOR_INCLUDED
#define YACK_WOVEN_QVECTOR_INCLUDED 1

#include "yack/woven/q-array.hpp"
#include "yack/container/writable.hpp"

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
            
            //! keep rational orthogonal component of source from *this
            template <typename SOURCE>
            bool ortho(writable<apq> &result, SOURCE &source) const {
                apq prod;
                for(size_t i=dimensions;i>0;--i) prod +=  coord[i] * source[i];
                const apq fac = prod/norm2;
                for(size_t i=dimensions;i>0;--i) result[i] = source[i] - fac * coord[i];
                return check_not_null(result);
            }

            const apz &norm2;




        private:
            YACK_DISABLE_ASSIGN(qvector);

        };

    }

}

#endif

