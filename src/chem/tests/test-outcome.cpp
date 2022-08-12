


#include "yack/chem/library.hpp"
#include "yack/chem/outcome.hpp"
#include "yack/utest/run.hpp"
#include "../../core/tests/main.hpp"
#include "yack/counting/part.hpp"

using namespace yack;
using namespace chemical;

namespace
{

    static inline void perform(const unsigned n, randomized::bits &ran)
    {
        for(unsigned nreac=0;nreac<=n;++nreac)
        {
            unsigned       iid=0;
            vector<string> rid(nreac,as_capacity);
            for(unsigned r=1;r<=nreac;++r)
            {
                const string id = 'a' + iid++;
                rid << id;
            }


            for(unsigned nprod=0;nprod<=n;++nprod)
            {
                if(!nreac && !nprod)
                {
                    continue;
                }
                YACK_ASSERT(nreac>0 || nprod>0);
                vector<string> pid(nprod,as_capacity);
                iid = nreac;

                for(unsigned p=1;p<=nprod;++p)
                {
                    const string id = 'a' + iid++;
                    pid << id;
                }
                std::cerr << rid << " <=> " <<  pid << std::endl;

                library lib;
                for(size_t i=1;i<=rid.size();++i) lib << rid[i];
                for(size_t i=1;i<=pid.size();++i) lib << pid[i];
                std::cerr << lib << std::endl;


            }

        }

    }

}


YACK_UTEST(outcome)
{
    randomized::rand_ ran;

    perform(3,ran);
    return 0;

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
        rmulops        mul;
        raddops        add;

        for(int p=-4;p<=4;++p)
        {
            const double K = pow(10.0,double(p));

            {
                lib.fill(Cini,ran);
                lib(std::cerr<< "C=", "", Cini);
                outcome res = outcome::study(usr,K,Cini,Cend,mul,add);
            }
        }



    }




}
YACK_UDONE()

