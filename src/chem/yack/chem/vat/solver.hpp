
#ifndef YACK_CHEM_SOLVER_INCLUDED
#define YACK_CHEM_SOLVER_INCLUDED 1

#include "yack/chem/eqs/aftermath.hpp"
#include "yack/chem/vat/cluster.hpp"
#include "yack/sequence/cxx-series.hpp"

namespace yack
{
    namespace Chemical
    {

        typedef vector<Aftermath,memory::dyadic>    Aftermaths;
        typedef cxx_series<Extents,memory::dyadic>  Limits;


        //______________________________________________________________________
        //
        //
        //! Solving coupled equilibria
        //
        //______________________________________________________________________
        class Solver
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            explicit Solver(const size_t maxClusterSize,
                            const size_t maximumSpecies);

            //! cleanup
            virtual ~Solver() noexcept;

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! solve top-level concentration/constants/scaling
            void  run(const xmlog                    &xml,
                      writable<Extended::Real>       &C0,
                      const Cluster                  &cluster,
                      const readable<Extended::Real> &K,
                      const readable<Extended::Real> &S);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            Equilibrium::Fund     eqsFund; //!< I/O for equilibria
            Species::Fund         spcFund; //!< I/O for species
            Equilibrium::CoopRepo running; //!< running equilibria
            Equilibrium::CoopRepo blocked; //!< blocked equilibria
            Equilibrium::CoopRepo founder; //!< basis for running equilibria
            Extents               extents; //!< scanning extents
            Limits                limits;
            Extended::Vector      Xi;      //!< running individual Xi, SubLevel
            Extended::Vector      Xa;      //!< |Xi|, SubLevel
            Extended::Vector      Corg;    //!< starting point, SubLevel
            Extended::Vector      Ctmp;    //!< trial point, SubLevel
            Extended::Vector      dC;      //!< delta C
            Extended::Matrix      Ceq;     //!< individual solutions, SubLevel
            Extended::Matrix      Phi;     //!< individual gradients, SubLevel
            Aftermaths            ams;     //!< aftermaths, SubLevel
            Extended::Add         xadd;    //!< helper
            Extended::Mul         xmul;    //!< helper


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Solver);
            void buildFounder(const xmlog &xml, const Cluster &cluster);
        };

    }
}

#endif
