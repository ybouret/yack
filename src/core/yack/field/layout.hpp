
//! \file

#ifndef YACK_FIELD_LAYOUT_INCLUDED
#define YACK_FIELD_LAYOUT_INCLUDED 1

#include "yack/type/out-of-reach.hpp"
#include <iostream>

namespace yack
{

    namespace kernel
    {
        //______________________________________________________________________
        //
        //
        //! base class for layouts
        //
        //______________________________________________________________________
        class layout
        {
        public:
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________

            const size_t space; //!< run-time dimension
            const size_t items; //!< number of linear items

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~layout() throw(); //!< cleanup

        protected:
            explicit layout(const size_t d) throw(); //!< setup
            explicit layout(const layout &) throw(); //!< cleanup


            void   finalize_(unit_t *lo, unit_t *hi, unit_t *width, unit_t *pitch)                    throw(); //!< post-construction setup
            bool   contains_(const unit_t *cr, const unit_t *lower, const unit_t *upper)        const throw(); //!< check if inside
            unit_t index_of_(const unit_t *cr, const unit_t *lower, const unit_t *pitch)        const throw(); //!< coord to index
            void   coord_of_(unit_t *arr, unit_t idx, const unit_t *lower, const unit_t *pitch) const throw(); //!< index to coord

        private:
            YACK_DISABLE_ASSIGN(layout);
        };

    }

    //______________________________________________________________________
    //
    //
    //! base class for layouts
    //
    //______________________________________________________________________
    template <typename COORD>
    class layout : public kernel::layout
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        static const size_t coords = sizeof(COORD)/sizeof(unit_t); //!< compile-time dimension

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~layout() throw() {}

        //! setup
        inline explicit layout(const COORD lo, const COORD up) throw() :
        kernel::layout(coords),
        lower(lo),
        upper(up),
        width(lo),
        pitch(lo)
        {
            finalize_((unit_t *) out_of_reach::address(&lower),
                      (unit_t *) out_of_reach::address(&upper),
                      (unit_t *) out_of_reach::address(&width),
                      (unit_t *) out_of_reach::address(&pitch));
        }

        //! copy
        inline   layout(const layout &other) throw() :
        kernel::layout(other),
        lower(other.lower),
        upper(other.upper),
        width(other.width),
        pitch(other.pitch)
        {
        }



        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________



        //! check if contains coord
        inline bool   contains(const COORD c) const throw() { return contains_((const unit_t*)&c, (const unit_t *)&lower, (const unit_t *)&upper); }

        //! COORD => index
        inline unit_t index_of(const COORD c) const throw() { return index_of_((const unit_t *)&c,(const unit_t *)&lower,(const unit_t *)&pitch); }

        //! index => COORD
        inline COORD  coord_of(const unit_t idx) const throw()
        {
            unit_t arr[4] = { 0,0,0,0 };
            coord_of_(arr,idx,(const unit_t *)&lower,(const unit_t *)&pitch);
            return *coerce_cast<COORD>(arr);
        }


        //! display
        inline friend
        std::ostream & operator<<(std::ostream &os, const layout &l)
        {
            os << '{' << l.lower << "->" << l.upper << " : #" << l.width << "=" << l.items << '}';
            return os;
        }

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const COORD lower; //!< lower coordinate
        const COORD upper; //!< upper coordinate
        const COORD width; //!< width
        const COORD pitch; //!< pitch

    private:
        YACK_DISABLE_ASSIGN(layout);
    };

}

#endif
