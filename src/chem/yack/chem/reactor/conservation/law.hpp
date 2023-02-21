//! \file

#ifndef YACK_CHEMICAL_CLAW_INCLUDED
#define YACK_CHEMICAL_CLAW_INCLUDED 1

#include "yack/chem/actor.hpp"
#include "yack/chem/types.hpp"
#include "yack/data/list/cxx.hpp"

namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! conservation law: more than two species
        //
        //______________________________________________________________________
        class claw : public object, public indexed
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit claw(const size_t i) noexcept; //!< setup
            virtual ~claw() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const list_of<actor> * operator->() const noexcept; //!< access
            void   add(const species &, const unsigned);       //!< add a new species
            bool   contains(const species &)    const noexcept; //!< check if species is in use
            void   finalize()                         noexcept; //!< compute nrm2
            bool   is_linked_to(const claw &)   const noexcept; //!< check is a species is shared

            //! specific display
            friend std::ostream & operator<<(std::ostream &, const claw &);

            //! compute excess
            /**
             - xs = <Q|Corg>
             - if(xs>=0) return false
             - Cout = Corg + (-xs) * Q / Q^2
             - load xadd with |dC|
             - return true
             */
            bool excess(const readable<double> &Corg,
                        writable<double>       &Cout,
                        raddops                &xadd) const;


            //! output colorized graph of conservation
            void viz(ios::ostream &) const;

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            claw *next; //!< for list
            claw *prev; //!< for list
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(claw);
            const unsigned     nrm2; //!< sum of square of weights
            cxx_list_of<actor> crew; //!< actors=species+wright
        };
    }
}

#endif
