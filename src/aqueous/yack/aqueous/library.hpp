
//! \file

#ifndef YACK_AQUEOUS_LIBRARY_INCLUDED
#define YACK_AQUEOUS_LIBRARY_INCLUDED 1

#include "yack/aqueous/species.hpp"
#include "yack/aqueous/com/gathering.hpp"
#include "yack/associative/suffix/set.hpp"
#include "yack/randomized/bits.hpp"

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
            static const int          pmin = -20;
            static const int          pmax = 1;
            static double             conc(randomized::bits &) noexcept;

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

            const species_db::tree_type & operator->() const noexcept;      //!< drill-down
            const species &               operator[](const string &) const; //!< get by name
            const species &               operator[](const char   *) const; //!< get by name
            YACK_PROTO_OSTREAM(library); //!< display
            void conc(writable<double> &C, randomized::bits &ran) const;

            //__________________________________________________________________
            //
            //! build a new species
            //__________________________________________________________________
            template <typename NAME> inline
            const species & operator()(NAME &name, const int algz)
            {
                return grow( new species(name,algz,sdb.size()+1) );
            }

            //__________________________________________________________________
            //
            // query species
            //__________________________________________________________________
            const species *query(const string &) const noexcept; //!< query by name
            const species *query(const char   *) const;          //!< query by name alias

            template <typename ARR>
            std::ostream & operator()(std::ostream &os,
                                      ARR          &arr,
                                      const char   *pfx = NULL) const
            {
                if(!pfx) pfx = "";
                os << '{' << std::endl;
                for(const snode *node=(*this)->head;node;node=node->next)
                {
                    const species &s = ***node;
                    pad(os << "  " << pfx << s,s) << " = " << arr[ s.indx[0] ] << std::endl;
                }
                os << '}';
                return os;
            }



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(library);
            species_db sdb;
            const species & grow(species *sp);

        };

    }

}

#endif

