#include "yack/utest/driver.hpp"

YACK_UTEST_DECL(80)
{
    YACK_UTEST(numeric);
    YACK_UTEST(triplet);
    YACK_UTEST(adder);
    YACK_UTEST(multiplier);

    YACK_UTEST(iota);
    YACK_UTEST(lu);
    YACK_UTEST(crout);
    YACK_UTEST(jacobi);
    YACK_UTEST(svd);
    YACK_UTEST(tridiag);
    YACK_UTEST(cyclic);
    YACK_UTEST(cholesky);
    YACK_UTEST(diagonalize);

    YACK_UTEST(rk4);
    YACK_UTEST(explicit);
    YACK_UTEST(app);

    YACK_UTEST(zroot);
    YACK_UTEST(zircon);

    YACK_UTEST(optimize);

    YACK_UTEST(drvs);
    YACK_UTEST(fjac);
    YACK_UTEST(functions);

    YACK_UTEST(poly);
    YACK_UTEST(chebyshev);
    YACK_UTEST(corr);
    YACK_UTEST(distrib);
   
    YACK_UTEST(fit_lam); 
    YACK_UTEST(fit);
    YACK_UTEST(gls);

    YACK_UTEST(area2d);

    YACK_UTEST(fitting_lambda);
    YACK_UTEST(fitting_vars);
    YACK_UTEST(fitting_samples);
}
YACK_UTEST_EXEC()
