//! \file

#ifndef YACK_CHEM_EQUILIBRIUM_INCLUDED
#define YACK_CHEM_EQUILIBRIUM_INCLUDED 1

#include "yack/chem/eqs/components.hpp"
#include "yack/data/dinky/core-repo.hpp"
#include "yack/data/dinky/solo-repo.hpp"

namespace yack
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! Equilibrium interfce
        //
        //______________________________________________________________________
        class Equilibrium : public spot_object, public counted, public Entity, public Components
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            typedef ark_ptr<string,Equilibrium> Pointer;  //!< alias
            typedef core_repo<Equilibrium>      CoreRepo; //!< alias
            typedef CoreRepo::node_type         Node;     //!< alias
            typedef solo_repo<Equilibrium>      SoloRepo; //!< alias
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

          
            YACK_PROTO_OSTREAM(Equilibrium);  //!< display
            Extended::Real K(const double t); //!< compute K at given time

            

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Equilibrium() noexcept; //!< cleanup

        protected:
            //! setup
            template <typename NAME> inline
            Equilibrium(const NAME &id) : Entity(id)
            {
            }
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Equilibrium);
            virtual Extended::Real getK(const double) = 0;
        };

        

    }

}

#endif

