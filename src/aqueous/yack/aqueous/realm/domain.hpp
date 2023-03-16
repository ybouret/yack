
//! \file

#ifndef YACK_AQUEOUS_DOMAIN_INCLUDED
#define YACK_AQUEOUS_DOMAIN_INCLUDED 1

#include "yack/aqueous/equilibria.hpp"
#include "yack/aqueous/realm/claw/preserved.hpp"
#include "yack/aqueous/realm/cluster.hpp"
#include "yack/container/matrix.hpp"
#include "yack/ios/xmlog.hpp"

namespace yack
{
    namespace aqueous
    {
        class library;
        
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
            void       create(const xmlog            &,
                              equilibria             &eqs,
                              const readable<double> &eks); //!< create all data

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t           N;         //!< this->size
            const size_t           M;         //!< live->size
            const size_t           L;         //!< lattice size
            const eq_node * const  last;      //!< last in 1..N eqs
            const sp_list          live;      //!< live species with sub-indices
            const conservations    laws;      //!< conservation laws
            const preserved        pack;      //!< groups of interlinked conservations
            const partition        reacting;  //!< reacting from lattice (make reaction)
            const partition        retaking;  //!< retaking from lattice (make positive)
            const eq_list          combining; //!< combining only
            const eq_list          splitting; //!< splitting only
            const eq_list          roaming;   //!< roaming equilibria
            const eq_list          defined;   //!< defined equilibria
            const sp_list          endless;   //!< endless species
            const sp_list          bounded;   //!< bounded species
            const matrix<int>      Nu;        //!< topology
            const matrix<int>      NuT;       //!< Nu'
            domain                *next;      //!< for domains
            domain                *prev;      //!< for domains

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(domain);

            //! index equilibria and species
            void make_dimensions(const xmlog &);

            //! deduce Nu and NuT
            void create_topology(const xmlog &);

            //! create manifold
            /**
             create all coupling, appending new eqs and updating last
             \param xml for output
             \param eqs new equilibria with global indidces are store
             \param eks external array of constants, must be allocated later
             */
            void create_manifold(const xmlog            &xml,
                                 equilibria             &eqs,
                                 const readable<double> &eks);

            void make_categories(const xmlog &); //!< categorize equilibria
            void build_conserved(const xmlog &); //!< build conservation
            void make_partitions(const xmlog &); //!< partition
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

