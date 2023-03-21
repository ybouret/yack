
//! \file

#ifndef YACK_AQUEOUS_DOMAIN_INCLUDED
#define YACK_AQUEOUS_DOMAIN_INCLUDED 1

#include "yack/spot-object.hpp"
#include "yack/aqueous/equilibria.hpp"
#include "yack/aqueous/realm/claw/preserved.hpp"
#include "yack/aqueous/realm/cluster.hpp"
#include "yack/container/matrix.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/ios/xmlog.hpp"

namespace yack
{
    namespace aqueous
    {
        typedef cxx_array<eq_list> eq_slots_; //!< base class for eq_slots

        //______________________________________________________________________
        //
        //
        //! slots to put equilibria by rank
        //
        //______________________________________________________________________
        class eq_slots : public object, public eq_slots_
        {
        public:
            explicit eq_slots(const size_t); //!< setup for 1..rank
            virtual ~eq_slots() noexcept; //!< cleanup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(eq_slots);
        };


        //______________________________________________________________________
        //
        //
        //! domain of connected equilibria
        //
        //______________________________________________________________________
        class domain : public spot_object, public eq_list
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
            explicit  domain(const equilibrium     &first,
                             const readable<bool>   &,
                             const readable<size_t> &); //!< setup with a first equilibrium
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

            //! register regular species of equilibrium
            void       reg_db(addrbook          &,
                              const equilibrium &) const;

            //! look for common regular species  in equilibirum
            bool       reg_in(const addrbook    &,
                              const equilibrium &) const noexcept;

            //! output GraphViz code for a given rank
            void viz(ios::ostream &, const size_t rank) const;

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const readable<bool>    &reg;       //!< flags for regular species
            const readable<size_t>  &grp;       //!< group indices for species
            const size_t             N;         //!< this->size
            const size_t             M;         //!< live->size
            const size_t             L;         //!< lattice size
            const eq_node * const    last;      //!< last in 1..N eqs
            const sp_list            live;      //!< live species with sub-indices
            const conservations      laws;      //!< conservation laws
            const preserved          pack;      //!< groups of interlinked conservations
            const partition          reacting;  //!< reacting from lattice (make reaction)
            const partition          retaking;  //!< retaking from lattice (make positive)
            const eq_list            combining; //!< combining only
            const eq_list            splitting; //!< splitting only
            const eq_list            roaming;   //!< other roaming equilibria
            const eq_list            defined;   //!< defined equilibria
            const sp_list            endless;   //!< endless species
            const sp_list            bounded;   //!< bounded species
            const matrix<int>        Nu;        //!< topology
            const matrix<int>        NuT;       //!< Nu'
            domain                  *next;      //!< for domains
            domain                  *prev;      //!< for domains
            const int                iviz;      //!< graphviz index
            const auto_ptr<eq_slots> slots;     //!< slots, post-allocated

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

