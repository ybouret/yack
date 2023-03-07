
//! \file

#ifndef YACK_AQUEOUS_LIBRARY_INCLUDED
#define YACK_AQUEOUS_LIBRARY_INCLUDED 1

#include "yack/aqueous/species.hpp"
#include "yack/aqueous/gathering.hpp"
#include "yack/associative/suffix/set.hpp"

namespace yack
{
    namespace aqueous
    {

        //______________________________________________________________________
        //
        //
        // types for species library
        //
        //______________________________________________________________________
        typedef suffix_set<string,species::ptr> species_db; //!< alias
        typedef species_db::knot_type           snode;      //!< alias

        //______________________________________________________________________
        //
        //
        //! a species is an entity with an algebraic charge
        //
        //______________________________________________________________________
        class library :  public gathering
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const char * const clid; //!< "library"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit library() noexcept; //!< setup empty
            virtual ~library() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            const species_db::tree_type * operator->() const noexcept;      //!< get internal tree
            const species_db::tree_type & operator*()  const noexcept;      //!< get internal tree
            const species &               operator[](const string &) const; //!< get by name
            const species &               operator[](const char   *) const; //!< get by name

            //__________________________________________________________________
            //
            //! build a new species
            //__________________________________________________________________
            template <typename NAME> inline
            const species & operator()(NAME &name, const int algz)
            {
                return grow( new species(name,algz,sdb.size()+1) );
            }




        private:
            YACK_DISABLE_COPY_AND_ASSIGN(library);
            species_db sdb;
            const species & grow(species *sp);

        };

    }

}

#endif

