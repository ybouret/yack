


#include "yack/chem/library.hpp"
#include "yack/chem/outcome.hpp"
#include "yack/utest/run.hpp"
#include "../../core/tests/main.hpp"
#include "yack/counting/part.hpp"
#include "yack/counting/comb.hpp"

using namespace yack;
using namespace chemical;

namespace
{

    enum species_kind
    {
        reactant,
        product
    };

    static inline
    void populate(library                &lib,
                  sequence<int>          &cof,
                  const species_kind      sid,
                  const readable<size_t> &arr,
                  size_t                 &idx)
    {
        const size_t num = arr.size();
        for(size_t i=1;i<=num;++i)
        {
            const string id = 'a' + idx++;
            lib << id;
            switch(sid)
            {
                case reactant: cof << -int(arr[i]); break;
                case product:  cof <<  int(arr[i]); break;
            }
        }
    }

    static inline
    void create(const readable<size_t> * const r,
                const readable<size_t> * const p,
                randomized::bits              &ran)
    {
        library     lib;
        components  cmp;
        rmulops     xmul;
        raddops     xadd;
        {
            vector<int> cof;
            {
                size_t      idx=0;

                if(r)
                {
                    populate(lib,cof,reactant,*r,idx);
                }
                if(p)
                {
                    populate(lib,cof,product,*p,idx);
                }
            }

            YACK_ASSERT(cof.size()==lib.size());
            const size_t M = lib.size();
            for(const snode *node=lib.head();node;node=node->next)
            {
                const species &sp = ***node;
                const int      nu = cof[*sp];
                cmp(sp,nu);
            }
            std::cerr << "---------- " << cmp << " ----------" << std::endl;
            for(int p=-4;p<=4;++p)
            {
                const double K = pow(10.0,p);
                std::cerr << "     ----- K = " << std::setw(15) << K << std::endl;
                vector<double> Cini(M,0);
                vector<double> Cend(M,0);
                {
                    const outcome res = outcome::study(cmp,K,Cini,Cend,xmul,xadd);
                    std::cerr << "C=" << Cini << " -> " << Cend << std::endl;
                }

                for(size_t neqz=M;neqz>0;--neqz)
                {
                    combination comb(M,neqz);
                    do
                    {
                        for(size_t iter=0;iter<4;++iter)
                        {
                            Cini.ld(0);
                            for(size_t i=neqz;i>0;--i)
                            {
                                Cini[ comb[i] ] = library::concentration(ran);
                            }
                            const outcome res = outcome::study(cmp,K,Cini,Cend,xmul,xadd);
                            std::cerr << "C=" << Cini << " -> " << Cend << std::endl;
                        }
                    }
                    while(comb.next());
                }


            }
        }
    }


    static inline void perform(const unsigned n, randomized::bits &ran)
    {
        YACK_ASSERT(n>0);

        partition r(n);

        r.boot();
        do
        {
            create(&r,NULL,ran);
            create(NULL,&r,ran);
        }
        while( r.next() );

        partition p(n);

        r.boot();
        do
        {
            p.boot();
            do
            {
                create(&r,&p,ran);
            }
            while(p.next());
        }
        while(r.next());



    }

}


YACK_UTEST(outcome)
{
    randomized::rand_ ran;

    perform(2,ran);
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

