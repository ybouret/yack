
//! \file

#ifndef YACK_CHEMICAL_EQUALIZER_INCLUDED
#define YACK_CHEMICAL_EQUALIZER_INCLUDED 1

#include "yack/chem/reactor/balance/boundaries.hpp"
#include "yack/chem/reactor.hpp"
#include "yack/data/dinky/solo-repo.hpp"

namespace yack {

    namespace chemical
    {


        typedef solo_repo<const squad> sq_repo_; //!< base class for private respository
        typedef sq_repo_::node_type    sq_node;  //!< alias



        //! private repository of equilibria for algorithm
        class sq_repo : public sq_repo_ {
        public:
            explicit sq_repo(const size_t n); //!< setup
            virtual ~sq_repo() throw();       //!< cleanup
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(sq_repo);
        };


        //______________________________________________________________________
        //
        //
        //! equalizing algorithm
        //
        //______________________________________________________________________
        class equalizer : public sp_fund
        {
        public:

            //! comply result
            enum status {
                balanced, //!< equilibrium is balanced
                bad_reac, //!< equilibriun has bad reactant(s)
                bad_prod, //!< equilibrium has bad product(s)
                bad_both  //!< equilibrium has bad both reactant(s) and product(s)
            };

            //! humar readable status
            static const char * status_text(const status) throw();

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~equalizer() throw();        //!< cleanup
            explicit equalizer(const reactor &); //!< setup
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            void adjust(writable<double> &C, const xmlog &xml); //!< adjust [reac|prod]_only using single wall
            void comply(writable<double> &C, const xmlog &xml); //!< comply to maximum positivity of conserved species
            
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const reactor    &cs;    //!< persistent reactor
            const size_t      mx;    //!< max actors for all the eqs
            frontier          wall;  //!< single wall 
            boundaries        reac;  //!< boundaries from reactant
            boundaries        prod;  //!< boundaries from products
            addrbook          eqdb;  //!< used
            sq_repo           pick;  //!< compatible squads
            matrix<double>    Ceqz;  //!< [LxM]
            cxx_array<double> gain;  //!< [L]
            raddops           xadd;  //!< to compute scores
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(equalizer);
            void adjust_reac(writable<double> &C, const eq_group  &reac_only, const xmlog &xml);
            void adjust_prod(writable<double> &C, const eq_group  &prod_only, const xmlog &xml);
            void adjust(writable<double> &C, const cluster &cc, const xmlog &xml);
            void comply(writable<double> &C, const cluster &cc, const xmlog &xml);

            status probe(const readable<double> &C, const equilibrium &eq);

            const equilibrium & comply_prod(const readable<double> &C,
                                            const equilibrium      &eq,
                                            const xmlog            &xml);
            

            const equilibrium &comply_reac(const readable<double> &C,
                                           const equilibrium      &eq,
                                           const xmlog            &xml);

            void comply_move(const frontier         &F,
                             const readable<double> &C,
                             const equilibrium      &eq,
                             const xmlog            &xml);

            static void locate_single_fence(frontier        &sf,
                                            const frontier  &limiting,
                                            const frontiers &amending,
                                            const xmlog     &xml);

            bool is_complying(const squad &) const throw();

            double gain_of(const squad &sq);


        };

    }

}

#endif
