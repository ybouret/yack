//! \file

#ifndef YACK_CHEM_ENTITY_INCLUDED
#define YACK_CHEM_ENTITY_INCLUDED 1

#include "yack/ios/fmt/entity.hpp"

namespace yack
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! Index Level to number Entities
        //
        //______________________________________________________________________
        enum IndexLevel
        {
            TopLevel = 0, //!< top level (Library/Equilibria)
            SubLevel = 1  //!< sub level (Subset/Cluster)
        };

        //______________________________________________________________________
        //
        //
        //! Entity has a name and different indices
        //
        //______________________________________________________________________
        class Entity : public ios::entity
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const size_t Levels = SubLevel+1; //!< number of levels

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup with a name
            template <typename NAME> inline
            explicit Entity(const NAME &id) : ios::entity(id), indx() { clr(); }

            //! cleanup
            virtual ~Entity() noexcept;

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t indx[Levels]; //!< indices

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Entity);
            void clr() noexcept;
        };

        typedef Entity::group Gathering; //!< alias to format entities names

    }
}

#endif


