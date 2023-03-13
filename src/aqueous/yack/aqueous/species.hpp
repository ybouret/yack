//! \file

#ifndef YACK_AQUEOUS_SPECIES_INCLUDED
#define YACK_AQUEOUS_SPECIES_INCLUDED 1

#include "yack/aqueous/com/entity.hpp"
#include "yack/data/dinky/coop-repo.hpp"
#include "yack/ptr/ark.hpp"

namespace yack
{
    namespace aqueous
    {
        //______________________________________________________________________
        //
        //
        //! a species is an entity with an algebraic charge
        //
        //______________________________________________________________________
        class species : public object, public counted, public entity
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            typedef ark_ptr<string,species> ptr;     //!< shared pointer
            static bool                     verbose; //!< global verbosity

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~species() noexcept; //!< cleanup

            //! setup name+charge+indx
            template <typename NAME> inline
            explicit species(NAME        &uuid,
                             const int    algz,
                             const size_t indx) :
            object(), counted(), entity(uuid,indx), z(algz), n(0)
            {

            }

            YACK_PROTO_OSTREAM(species); //!< display [name]


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const int    z; //!< algebraic charge
            const size_t n; //!< number of references

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(species);
        };


        typedef coop_repo<const species> sp_repo;   //!< alias
        typedef sp_repo::node_type       sp_node;   //!< alias
        typedef sp_repo::zpool_type      sp_zpool;  //!< alias
        typedef sp_repo::proxy_type      sp_proxy;  //!< alias

    }

}

#endif

