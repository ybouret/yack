
#include "yack/counting/binary.hpp"
#include "yack/type/destruct.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{

    binary_spots:: ~binary_spots() noexcept
    {
        restart();
    }

    static inline
    cardinality_t card_for(size_t maxbits)
    {
        static const char          fn[] = " for binary spots";
        static const cardinality_t one  = 1;
        static const cardinality_t cmb  = sizeof(cardinality_t) * 8 -1;
        if(maxbits>cmb) throw libc::exception(EDOM,"too many bits%s",fn);
        if(maxbits<=0)  throw libc::exception(EDOM,"no bits%s",fn);
        return one << maxbits;
    }

    binary_spots:: binary_spots(const size_t maxbits) :
    counting(card_for(maxbits)),
    comb(NULL),
    bmax(maxbits),
    wksp()
    {

    }

    size_t binary_spots::capacity() const noexcept
    {
        return bmax;
    }

    void binary_spots:: restart() noexcept
    {
        if(comb)
        {
            out_of_reach::naught( destructed(comb) );
            comb = 0;
        }
    }

    void binary_spots:: on_boot() noexcept
    {
        restart();
    }

    size_t binary_spots:: size() const noexcept
    {
        return comb ? comb->size() : 0;
    }

    const size_t & binary_spots:: operator[](const size_t indx) const noexcept
    {
        assert(comb);
        return (*comb)[indx];
    }

    bool binary_spots:: on_next()
    {
        if(comb)
        {
            if(comb->next())
            {
                return true;
            }
            else
            {
                // end of current comb
                size_t k = comb->size();
                restart();
                if(++k>bmax)
                {
                    return false;
                }
                else
                {
                    comb = new ( *wksp ) combination(bmax,k);
                    return true;
                }
            }
        }
        else
        {
            // first
            comb = new ( *wksp ) combination(bmax,1);
            return true;
        }
    }

}
