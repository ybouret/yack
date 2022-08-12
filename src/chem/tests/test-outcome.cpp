


#include "yack/chem/library.hpp"
#include "yack/chem/outcome.hpp"
#include "yack/utest/run.hpp"
#include "../../core/tests/main.hpp"
#include "yack/type/boolean.h"

using namespace yack;
using namespace chemical;


YACK_UTEST(outcome)
{
    randomized::rand_ ran;
    library           lib;
    components        usr;

    if(argc>1)
    {
        usr(lib,argv[1]);

        std::cerr << lib << std::endl;
        std::cerr << usr << std::endl;

        const size_t M = lib.size();
        vector<double> Cini(M,0);
        vector<double> Cend(M,-1);
        rmulops        ops;


        for(int p=-4;p<=4;++p)
        {
            const double K = pow(10.0,double(p));

            {
                lib.fill(Cini,ran);
                lib(std::cerr<< "C=", "", Cini);
                outcome res = outcome::study(usr,K,Cini,Cend,ops);
            }
        }




    }




}
YACK_UDONE()

