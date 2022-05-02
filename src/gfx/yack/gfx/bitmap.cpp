#include "yack/gfx/bitmap.hpp"

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
    }
}
