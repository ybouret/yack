
#include "yack/apex/natural.hpp"
#include "yack/system/endian.hpp"
#include "yack/arith/align.hpp"

namespace yack
{
    namespace apex
    {

        static const size_t _bit[8] =
        {
            size_t(1) << 0,
            size_t(1) << 1,
            size_t(1) << 2,
            size_t(1) << 3,
            size_t(1) << 4,
            size_t(1) << 5,
            size_t(1) << 6,
            size_t(1) << 7
        };

        bool natural::bit(const size_t ibit) const throw()
        {

            assert(ibit<bits());
            size_t        iB  = ibit>>3;       assert(iB<bytes);
            const size_t  ib = ibit - (iB<<3); assert(ib<8);
            const uint8_t B  = (*this)[++iB];
            return 0 != (B & _bit[ib]);
        }


        namespace
        {
            typedef   uint8_t & (*apn_get_proc)(natural::word_type &, const size_t);

            static inline   uint8_t & getBE(natural::word_type &w, const size_t at) throw()
            {
                assert(at<sizeof(w));
                uint8_t *p= (uint8_t *)&w;
                return p[(sizeof(w)-1)-at];
            }

            static inline   uint8_t & getLE(natural::word_type &w, const size_t at) throw()
            {
                assert(at<sizeof(w));
                uint8_t *p = (uint8_t *)&w;
                return p[at];
            }
        }



        natural natural:: shl1(const size_t shift)
        {
            static const apn_get_proc proc =  endianness::BE() ? getBE : getLE;

            const size_t bits  = shift+1;
            const size_t bytes = YACK_ALIGN_ON(8,bits)>>3;
            const size_t words = YACK_ALIGN_ON(word_bits,bits)>>ilog2<word_bits>::value;
            const size_t ibyte = bytes-1;
            const size_t iword = words-1;
            const size_t ibit  = shift - (ibyte<<3); assert(ibit<8);

            //std::cerr << "shift=" << shift << " => bits = " << bits << " => bytes=" << bytes << ", words=" << words << " => ibit=" << ibit << std::endl;

            natural res(words,as_capacity);

            res.bytes=bytes;
            res.words=words;

            proc(res.word[iword],ibyte-((ibyte>>word_exp2)<<word_exp2)) = _bit[ibit];

            return res;


        }


    }

}

