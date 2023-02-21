#include "yack/apex/natural.hpp"
#include "yack/system/endian.hpp"

namespace yack
{
    namespace apex
    {
        namespace
        {
            typedef const uint8_t & (*apn_get_proc)(const natural::word_type &, const size_t);

            static inline const uint8_t & getBE(const natural::word_type &w, const size_t at) noexcept
            {
                assert(at<sizeof(w));
                const uint8_t *p= (const uint8_t *)&w;
                return p[(sizeof(w)-1)-at];
            }

            static inline const uint8_t & getLE(const natural::word_type &w, const size_t at) noexcept
            {
                assert(at<sizeof(w));
                const uint8_t *p= (const uint8_t *)&w;
                return p[at];
            }
        }


        const uint8_t & natural:: operator[](const size_t indx) const noexcept
        {
            static const apn_get_proc proc =  endian::BE() ? getBE : getLE;

            assert(indx>0);
            assert(indx<=bytes);

            const size_t iB = indx-1;
            const size_t iw = iB>>word_exp2;
            const size_t at = iB-(iw<<word_exp2);
            return proc(word[iw],at);
        }

    }

}
