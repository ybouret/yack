
//! \file

#ifndef YACK_FIELD_LAYOUT_INCLUDED
#define YACK_FIELD_LAYOUT_INCLUDED 1

#include "yack/type/out-of-reach.hpp"

namespace yack
{

    namespace kernel
    {
        class layout
        {
        public:
            virtual ~layout() throw();

            const size_t dims;
            const size_t items;

        protected:
            explicit layout(const size_t d) throw();
            explicit layout(const layout &) throw();

            void setup(unit_t *lo, unit_t *hi, unit_t *w) throw();

        private:
            YACK_DISABLE_ASSIGN(layout);
        };

    }

    template <typename COORD>
    class layout : public kernel::layout
    {
    public:
        static const size_t dimension = sizeof(COORD)/sizeof(unit_t);

        inline virtual ~layout() throw() {}

        inline explicit layout(const COORD lo, const COORD up) throw() :
        kernel::layout(dimension),
        lower(lo),
        upper(up),
        width()
        {
            setup((unit_t *) out_of_reach::address(&lower),
                  (unit_t *) out_of_reach::address(&upper),
                  (unit_t *) out_of_reach::address(&width) );
        }

        inline   layout(const layout &other) throw() :
        kernel::layout(other),
        lower(other.lower),
        upper(other.upper),
        width(other.width)
        {
        }
        

        const COORD lower;
        const COORD upper;
        const COORD width;

    private:
        YACK_DISABLE_ASSIGN(layout);
    };

}

#endif
