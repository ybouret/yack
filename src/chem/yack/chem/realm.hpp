//! \file

#ifndef YACK_CHEMICAL_REALM_INCLUDED
#define YACK_CHEMICAL_REALM_INCLUDED 1

#include "yack/chem/realm/domain.hpp"
#include "yack/chem/library.hpp"
#include "yack/sequence/cxx-array.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! chemical system
        //
        //______________________________________________________________________
        class realm : public domains, public xmlog
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit realm(const library          &user_lib,
                           const equilibria       &user_eqs,
                           const readable<double> &user_K); //!< setup
            virtual ~realm() noexcept;                      //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! output multiple GraphViz layers
            void   graphviz(const string &rootname,
                            const size_t  depth=0) const;

            //! return largest domain size
            size_t largest_domain_size() const noexcept;

            void   computeK(writable<double> &K, const double t); //!< K is allocated post creation

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const library          &lib; //!< persistent library
            equilibria              eqs; //!< original+manifold
            const cxx_array<bool>   reg; //!< regular species
            const cxx_array<size_t> grp; //!< group index for species
            const matrix<int>       Nu;  //!< global topology
            const matrix<int>       NuT; //!< global Nu'

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(realm);

            //! group equilibria in domains
            void build_domains(const xmlog &);
        };

    }

}

#endif

