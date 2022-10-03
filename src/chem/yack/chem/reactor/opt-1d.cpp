
#include "yack/chem/reactor.hpp"
#include "yack/math/opt/optimize.hpp"


namespace yack
{
    using namespace math;

    namespace chemical
    {
        
        double reactor:: Optimized1D(const double H0)
        {
            triplet<double> U = { 0,  -1, 1.0 };
            triplet<double> H = { H0, -1, Hamiltonian(Cend) };
            optimize::run_for(*this, U, H, optimize::inside);
            return H.b;
        }
    }
}


