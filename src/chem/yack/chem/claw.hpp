
//! \file

#ifndef YACK_CHEMICAL_CLAW_INCLUDED
#define YACK_CHEMICAL_CLAW_INCLUDED 1

#include "yack/chem/actors.hpp"
#include "yack/math/adder.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! conservation law = actors
        //
        //______________________________________________________________________
        class conservation_law : public object, public actors
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit conservation_law() throw(); //!< setup
            virtual ~conservation_law() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! specific display
            friend std::ostream & operator<<(std::ostream &os, const conservation_law &self);

            void   finalize(const size_t i) throw();                     //!< setup nrm2 and inx
            double evaluate(const readable<double> &C, raddops &) const; //!< evaluate excess

            //! regulate
            /**
             - if evaluate() <=0, return false
             - if evaluate() > 0: copy source, correct target, load xadd, return treu
             */
            bool   regulate(writable<double>       &target,
                            const readable<double> &source,
                            raddops                &xadd) const;

            //! update injected concentration from source to target
            void   injected(writable<double>       &inset,
                            const readable<double> &source,
                            const readable<double> &target) const throw();

            //! access index
            size_t operator*() const throw() { return indx; }
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            conservation_law *next; //!< for list
            conservation_law *prev; //!< for list


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(conservation_law);
            const unsigned    nrm2; //!< sum of squared coefficients
            const size_t      indx; //!< index
        };

        //______________________________________________________________________
        //
        //
        //! base class for conservations laws
        //
        //______________________________________________________________________
        typedef cxx_list_of<conservation_law>     conservation_laws_;


        //______________________________________________________________________
        //
        //
        //! hold list of c-laws
        //
        //______________________________________________________________________
        class conservation_laws : public object, public conservation_laws_
        {
        public:
            //! setup empty
            explicit conservation_laws() throw() : object(), conservation_laws_() {}

            //! cleanup
            virtual ~conservation_laws() throw() {}

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(conservation_laws);
        };
    }

}

#endif

