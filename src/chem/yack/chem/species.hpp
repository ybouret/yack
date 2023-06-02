//! \file

#ifndef YACK_CHEM_SPECIES_INCLUDED
#define YACK_CHEM_SPECIES_INCLUDED 1

#include "yack/chem/com/entity.hpp"
#include "yack/object.hpp"
#include "yack/spot-object.hpp"
#include "yack/counted.hpp"
#include "yack/ptr/ark.hpp"
#include "yack/data/dinky/core-repo.hpp"
#include "yack/proto-ostream.hpp"

namespace yack
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! a species is an Entity with a charge
        //
        //______________________________________________________________________
        class Species : public spot_object, public  Entity, public counted
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            typedef ark_ptr<string,Species> Pointer;  //!< alias
            static  bool                    Verbose;  //!< to debug
            typedef core_repo<Species>      CoreRepo; //!< alias
            typedef CoreRepo::node_type     Node;     //!< alias

            //! category of species
            enum Category
            {
                Conserved, //!< in a conservation law
                Unbounded  //!< not involved in conservation
            };

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            template <typename NAME> inline
            explicit Species(const NAME &id,
                             const int   zz) :
            Entity(id),
            z(zz) {}

            virtual ~Species() noexcept; //!< cleanup
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            YACK_PROTO_OSTREAM(Species); //! display as '[name]'

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const int z; //!< algebraic charge

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Species);

        };


        
    }

}

#endif

