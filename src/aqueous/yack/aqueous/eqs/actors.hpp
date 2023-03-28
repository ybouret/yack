//! \file

#ifndef YACK_AQUEOUS_ACTORS_INCLUDED
#define YACK_AQUEOUS_ACTORS_INCLUDED 1

#include "yack/aqueous/eqs/actor.hpp"
#include "yack/data/list/cxx.hpp"

namespace yack
{
    namespace aqueous
    {


        //______________________________________________________________________
        //
        //
        //! base class for actors
        //
        //______________________________________________________________________
        typedef cxx_list_of<actor> actors_;



        //______________________________________________________________________
        //
        //
        //! list of actors: reactants or product
        //
        //______________________________________________________________________
        class actors : public object, public actors_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit actors() noexcept; //!< setup empty
            virtual ~actors() noexcept; //!< cleanup
            actors(const actors &);     //!< copy using actor copy constructor

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            YACK_PROTO_OSTREAM(actors); //!< display
            string to_string() const;   //!< stringify

            //! append terms to xmul
            void mass_action(const readable<double> &C,
                             const index_level       I,
                             cameo::mul<double>     &xmul) const;

            //! append terms to xmul, using C+nu*xi
            /**
             \warning take care of sign of xi!
             */
            void mass_action(const readable<double> &C,
                             const index_level       I,
                             const double            xi,
                             cameo::mul<double>     &xmul) const;

           

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            actors *next; //!< for pool
            
        private:
            YACK_DISABLE_ASSIGN(actors);
        };

    }

}

#endif

