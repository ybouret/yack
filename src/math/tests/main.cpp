#include "yack/utest/driver.hpp"

YACK_UTEST_DECL(80)
{
    YACK_UTEST(numeric);
    YACK_UTEST(triplet);
    YACK_UTEST(adder);
    YACK_UTEST(multiplier);

    YACK_UTEST(iota);
    YACK_UTEST(crout);
    YACK_UTEST(lss);
    YACK_UTEST(jacobi);
    YACK_UTEST(svd);
    YACK_UTEST(tridiag);
    YACK_UTEST(cyclic);
    YACK_UTEST(cholesky);
    YACK_UTEST(diagonalize);
    YACK_UTEST(ortho);

    YACK_UTEST(rk4);
    YACK_UTEST(explicit);
    YACK_UTEST(app);

    YACK_UTEST(zroot);
    YACK_UTEST(zircon);

    YACK_UTEST(locate);
    YACK_UTEST(optimize);

    YACK_UTEST(drvs);
    YACK_UTEST(fjac);
    YACK_UTEST(functions);

    YACK_UTEST(poly);
    YACK_UTEST(chebyshev);
    YACK_UTEST(corr);
    YACK_UTEST(distrib);
   
    YACK_UTEST(area2d);

    YACK_UTEST(fitting_lambda);
    YACK_UTEST(fitting_vars);
    YACK_UTEST(fitting_sample);
    YACK_UTEST(fitting_samples);
    YACK_UTEST(fitting_gls);
    YACK_UTEST(fitting_ode);
    YACK_UTEST(fitting_ode2);

    YACK_UTEST(proto_diff1d);

}
YACK_UTEST_EXEC()
