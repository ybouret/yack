
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
#include "yack/associative/key-mapper.hpp"

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

        typedef key_mapper<size_t,size_t> index_map;

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

            //! conc += NuT * xi
            void add(writable<double>       &conc,
                     const readable<double> &xi,
                     cameo::add<double>     &xadd) const
            {
                std::cerr << "domain: xi=" << xi << ", NuT=" << NuT << std::endl;
                size_t J=1;
                for(const sp_node *sn=live.head;sn;sn=sn->next,J++)
                {
                    const species       &s = ***sn;
                    const size_t         j = s.indx[top_level];
                    const readable<int> &f = NuT[J];
                    size_t               I = 1;
                    xadd.free();
                    for(const eq_node *en=head;en!=last;en=en->next,I++)
                    {
                        const equilibrium &eq = ***en;
                        const size_t       i  = eq.indx[top_level];
                        xadd.push(f[I] * xi[i]);
                    }
                    conc[j] += xadd.sum();
                }
            }
            

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
            const matrix<int>        Nu;        //!< [NxM] topology
            const matrix<int>        NuT;       //!< [MxN] Nu'
            const index_map          eqmap;     //!< global/local
            const index_map          spmap;     //!< global/local
            domain                  *next;      //!< for domains
            domain                  *prev;      //!< for domains
            const int                iviz;      //!< graphviz index
            const auto_ptr<eq_slots> slots;     //!< slots, post-allocated
            const gathering          eqfmt;     //!< local format for equilibria
            const gathering          spfmt;     //!< local format for species
            
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
            void make_index_maps(const xmlog &); //!< make mapping
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

