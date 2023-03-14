
//! \file

#ifndef YACK_AQUEOUS_DOMAIN_INCLUDED
#define YACK_AQUEOUS_DOMAIN_INCLUDED 1

#include "yack/aqueous/equilibria.hpp"
#include "yack/data/dinky/core-list.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{
    namespace aqueous
    {
        //______________________________________________________________________
        //
        //
        //! domain of connected equilibria
        //
        //______________________________________________________________________
        class domain : public large_object, public eq_list
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const char * const clid; //!< "domain"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit  domain(const equilibrium &first); //!< setup with a first equilibrium
            virtual  ~domain() noexcept;                //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool       accepts(const equilibrium &) const noexcept;        //!< if a species is shared
            bool       accepts(const domain      &) const noexcept;        //!< if a species is shared
            static int compare(const eq_node *, const eq_node *) noexcept; //!< by increasing indx[0]
            void       create(const xmlog &);                              //!< create all data

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t           N;       //!< this->size
            const size_t           M;       //!< live->size
            const sp_list          live;    //!< live species with sub-indices
            const eq_list          roaming; //!< roaming equilibria
            const eq_list          defined; //!< defined equilibria
            const sp_list          endless; //!< endless species
            const sp_list          bounded; //!< bounded species
            const matrix<int>      Nu;      //!< topology
            const matrix<int>      NuT;     //!< Nu'
            domain                *next;    //!< for domains
            domain                *prev;    //!< for domains

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(domain);
            void make_dimensions(const xmlog &); //!< gather equilibria and species, indexing...
            void create_topology(const xmlog &); //!< deduce Nu
            void create_manifold(const xmlog &);
            void build_conserved(const xmlog &, const matrix<int>&); //!< build conservation
        };

        //______________________________________________________________________
        //
        //
        //! list of domains
        //
        //______________________________________________________________________
        typedef cxx_list_of<domain> domains;

    }

}

#endif

