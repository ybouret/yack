
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

            const size_t dims;  //!< run-time dimension
            const size_t items; //!< number of linear items

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~layout() throw(); //!< cleanup

        protected:
            explicit layout(const size_t d) throw(); //!< setup
            explicit layout(const layout &) throw(); //!< cleanup

            //! post-construction setup
            void setup(unit_t *lo, unit_t *hi, unit_t *w) throw();

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
        static const size_t dimension = sizeof(COORD)/sizeof(unit_t); //!< compile-time dimension

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~layout() throw() {}

        //! setup
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

        //! copy
        inline   layout(const layout &other) throw() :
        kernel::layout(other),
        lower(other.lower),
        upper(other.upper),
        width(other.width)
        {
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! display
        inline friend
        std::ostream & operator<<(std::ostream &os, const layout &l)
        {
            os << '{' << l.lower << "->" << l.upper << " : #" << l.witdh << "=" << l.items << '}';
            return os;
        }

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const COORD lower; //!< lower coordinate
        const COORD upper; //!< upper coordinate
        const COORD width; //!< width
        

    private:
        YACK_DISABLE_ASSIGN(layout);
    };

}

#endif
