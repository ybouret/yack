//! \file

#ifndef YACK_CHEMICAL_REACTOR_INCLUDED
#define YACK_CHEMICAL_REACTOR_INCLUDED 1

#include "yack/chem/realm/domain.hpp"
#include "yack/chem/eqs/xlimits.hpp"

namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! reactor to find solution of a valid set of concentratipons
        //
        //______________________________________________________________________
        class reactor : public spot_object
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit reactor(const domain   &,
                             const eq_proxy &); //!< setup
            virtual ~reactor() noexcept;      //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! from global C/K
            void solve(const xmlog            &xml,
                       writable<double>       &C,
                       const readable<double> &K);

            double excess(const readable<double> &C);
            double excess(const cluster &);

            //! excess in Corg * (1-u) + Cend * u
            double operator()(const double u);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const domain      &dom;     //!< persitent domain
            xlimits            xlim;    //!< to find limits
            cameo::add<double> xadd;    //!< for internal evaluations
            cameo::mul<double> xmul;    //!< for internal evaliations
            cxx_array<double>  Corg;    //!< [M] compact sub_level concentrations
            cxx_array<double>  Cend;    //!< [M] for computations
            cxx_array<double>  Ctmp;    //!< [M] for computations
            cxx_array<double>  Korg;    //!< [L] compact sub_level constants
            cxx_array<bool>    blocked; //!< [L]
            cxx_array<bool>    running; //!< [L]
            cxx_array<double>  Xi;      //!< [L] compact Xi
            cxx_array<double>  sigma;   //!< [L]
            cxx_array<double>  shift;   //!< [L] numerical mass action error
            matrix<double>     Cs;      //!< [LxM]
            cxx_array<xlimits> Xl;      //!< [N]
            eq_repo            active;
            eq_repo            bundle;
            eq_repo            subset;
            
            reactor *next; //!< for list
            reactor *prev; //!< for list

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(reactor);

            void   find_active(const xmlog &xml);
            bool   find_global(const xmlog &xml, const double X0);
            void   move_global(const xmlog &xml);
            void   local_steps(const xmlog &xml, const double X0);
        };
    }

}

#endif
