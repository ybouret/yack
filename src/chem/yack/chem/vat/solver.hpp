
#ifndef YACK_CHEM_SOLVER_INCLUDED
#define YACK_CHEM_SOLVER_INCLUDED 1

#include "yack/chem/eqs/aftermath.hpp"
#include "yack/chem/vat/cluster.hpp"

namespace yack
{
    namespace Chemical
    {

        typedef vector<Aftermath,memory::dyadic> Aftermaths;

        class Solver
        {
        public:
            explicit Solver(const size_t maxClusterSize,
                            const size_t maximumSpecies);
            virtual ~Solver() noexcept;

            //! solve top-level
            void  run(const xmlog                    &xml,
                      writable<Extended::Real>       &C0,
                      const Cluster                  &cluster,
                      const readable<Extended::Real> &K,
                      const readable<Extended::Real> &S);

            Equilibrium::Fund     eqsFund;
            Species::Fund         spcFund;
            Equilibrium::CoopRepo running;
            Equilibrium::CoopRepo blocked;
            Extents               extents;
            Extended::Vector      Corg;
            Extended::Vector      Ctmp;
            Extended::Matrix      Ceq;
            Extended::Matrix      Phi;
            Aftermaths            ams;
            Extended::Add         xadd;
            Extended::Mul         xmul;


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Solver);
        };

    }
}

#endif

