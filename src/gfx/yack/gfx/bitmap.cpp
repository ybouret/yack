#include "yack/gfx/bitmap.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace graphic
    {
        bitmap:: ~bitmap() throw()
        {
        }

        bitmap::bitmap(const bitmap &other) throw() :
        article(),
        metrics(other),
        data(other.data),
        rows(other.rows)
        {
        }


        bitmap:: bitmap(const unit_t W, const unit_t H, const unit_t D) :
        article(),
        metrics(W,H,D),
        data( new bitfield(n*d)              ),
        rows( new bitrows(*this,data->entry) )
        {

        }

        static inline
        bitfield * check_bitfield(bitfield *F, const metrics &info)
        {
            static const char fn[] = "bitmap: ";

            if(!F)
            {
                throw exception("%sNULL bitfield",fn);
            }

            if(F->bytes<info.b)
            {
                delete F;
                throw exception("%sinvalid bitfield",fn);
            }

            return F;
        }

        bitmap:: bitmap(bitfield    *F,
                        const unit_t W,
                        const unit_t H,
                        const unit_t D) :
        article(),
        metrics(W,H,D),
        data( check_bitfield(F,*this) ),
        rows( new bitrows(*this,data->entry) )
        {
        }

        const bitrow & bitmap:: line(const unit_t j) const throw()
        {
            assert(j>=0);
            assert(j<h);
            return rows->row[j];
        }


        bitrow & bitmap:: line(const unit_t j)   throw()
        {
            assert(j>=0);
            assert(j<h);
            return rows->row[j];
        }
    }
}
