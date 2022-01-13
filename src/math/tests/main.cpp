#include "yack/utest/driver.hpp"

YACK_UTEST_DECL(80)
{
    YACK_UTEST(numeric);
    YACK_UTEST(triplet);

    YACK_UTEST(lu);
    YACK_UTEST(jacobi);
    YACK_UTEST(svd);
    YACK_UTEST(tridiag);
    YACK_UTEST(cyclic);
    YACK_UTEST(cholesky);
    YACK_UTEST(diagonalize);

    YACK_UTEST(rk4);
    YACK_UTEST(explicit);

    YACK_UTEST(zroot);
    YACK_UTEST(zircon);

    YACK_UTEST(minimize);

    YACK_UTEST(drvs);
    YACK_UTEST(fjac);

    YACK_UTEST(poly);
    YACK_UTEST(chebyshev);
    YACK_UTEST(corr);
    
    YACK_UTEST(fit);
    YACK_UTEST(gls);

}
YACK_UTEST_EXEC()
