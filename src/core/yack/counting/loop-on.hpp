
//! \file

#ifndef YACK_COUNTING_LOOP_ON_INCLUDED
#define YACK_COUNTING_LOOP_ON_INCLUDED 1

#include "yack/counting/mloop.hpp"
#include "yack/type/out-of-reach.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! loop on coordinates
    //
    //__________________________________________________________________________
    template <typename COORD>
    class loop_on : public mloop<unit_t>
    {
    public:
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________
        typedef yack::mloop<unit_t> self_loop;                                 //!< alias
        static const size_t         dimensions = sizeof(COORD)/sizeof(unit_t); //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~loop_on() throw() {}

        //! setup from coordinates
        inline explicit loop_on(const COORD lo, const COORD up) :
        self_loop(coerce_cast<unit_t,COORD>(&lo),
                  coerce_cast<unit_t,COORD>(&up),
                  dimensions),
        value(*coerce_cast<COORD,unit_t>(&values[1])),
        lower(*coerce_cast<COORD,unit_t>(&origin[1])),
        upper(*coerce_cast<COORD,unit_t>(&target[1]))
        {
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________


        //! get content
        inline const COORD & operator*() const throw() { return value; }

        //! reset by coordinates
        inline void reset(const COORD lo, const COORD up)
        {
            self_loop::reset(coerce_cast<unit_t,COORD>(&lo),
                             coerce_cast<unit_t,COORD>(&up));
        }


        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const COORD &value; //!< alias values
        const COORD &lower; //!< alias origin
        const COORD &upper; //!< alias target



    private:
        YACK_DISABLE_COPY_AND_ASSIGN(loop_on);
    };

}

#endif

