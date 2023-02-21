
//! \file

#ifndef YACK_MATH_CORR_INCLUDED
#define YACK_MATH_CORR_INCLUDED 1

#include "yack/sequence/list.hpp"
#include "yack/type/v2d.hpp"
#include "yack/math/adder.hpp"


namespace yack
{
    namespace math
    {
        //______________________________________________________________________
        //
        //
        //! computing correlation coefficients
        //
        //______________________________________________________________________
        template <typename T>
        class correlation : public list< v2d<T> >
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef v2d<T>      point;   //!< alias
            typedef list<point> points;  //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit correlation() noexcept; //!< setup
            virtual ~correlation() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            T             operator()(void);                 //!< compute with current list
            correlation & operator()(const T x, const T y); //!< add a new point

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            adder<T> xadd; //!< extended additions

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(correlation);
        };

    }

}

#endif
