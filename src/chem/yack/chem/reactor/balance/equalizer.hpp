
//! \file

#ifndef YACK_CHEMICAL_EQUALIZER_INCLUDED
#define YACK_CHEMICAL_EQUALIZER_INCLUDED 1

#include "yack/chem/reactor/balance/boundaries.hpp"
#include "yack/chem/reactor.hpp"
#include "yack/data/dinky/solo-repo.hpp"

namespace yack {

    namespace chemical
    {

        
        //______________________________________________________________________
        //
        //
        //! equalizing algorithm
        //
        //______________________________________________________________________
        class equalizer : public sp_fund
        {
        public:

            typedef solo_repo<const equilibrium> er_type_;
            class er_type : public er_type_ {
            public:
                explicit er_type(const size_t n);
                virtual ~er_type() throw();
                YACK_PROTO_OSTREAM(er_type);
            };


            enum status {
                balanced,
                bad_reac,
                bad_prod,
                bad_both
            };

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


            void adjust(writable<double> &C); //!< adjust [reac|prod]_only using single fence
            void comply(writable<double> &C); //!< comply to maximum positivity of conserved species
            
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const reactor &cs;    //!< persistent reactor
            const size_t   mx;    //!< max actors
            frontier       sf;    //!< single fence
            boundaries     reac;  //!< boundaries from reactant
            boundaries     prod;  //!< boundaries from products
            er_type           used;  //!< used equilibria for a cycle
            matrix<double>    Ceqz;  //!< [LxM]
            cxx_array<double> gain;  //!< [L]
            raddops           xadd;  //!< to compute scores
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(equalizer);
            void adjust_reac(writable<double> &C, const eq_group  &reac_only);
            void adjust_prod(writable<double> &C, const eq_group  &prod_only);
            void adjust(writable<double> &C, const cluster &cc);
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

        };

    }

}

#endif
