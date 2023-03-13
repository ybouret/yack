
#include "yack/aqueous/weasel/designer.hpp"
#include "yack/aqueous/eqs/aftermath.hpp"

#include "yack/utest/run.hpp"
#include "yack/ios/icstream.hpp"
#include "yack/math/iota.hpp"

#include "yack/counting/part.hpp"

using namespace yack;
using namespace aqueous;
using namespace math;



static inline
void add_comp(equilibrium    &eq,
              library        &lib,
              const schedule &comp,
              unsigned       &indx,
              int             cfac)
{
    for(size_t i=1;i<=comp.size();++i)
    {
        ++indx;
        const string   name = vformat("a%u",indx);
        const species &sp   = lib(name,0);
        const size_t   nu   = comp[i];
        eq( cfac * int(nu), sp);
    }
}



static inline
void build_eq(equilibrium    &eq,
              library        &lib,
              const schedule *reac,
              const schedule *prod)
{
    unsigned indx=0;
    if(reac) {
        add_comp(eq,lib,*reac,indx,-1);
    }

    if(prod) {
        add_comp(eq,lib,*prod,indx,1);

    }
}

static const char * const AM = "am.dat";
static unsigned           AI = 0;

static inline
void perform(const double      K,
             const schedule   *reac,
             const schedule   *prod,
             randomized::bits &ran)
{
    cameo::mul<double> xmul;
    cameo::add<double> xadd;
    xlimits            xlim;

    const_equilibrium eq("eq",K,1);
    library           lib;
    build_eq(eq,lib,reac,prod);
    eq.display(std::cerr << "\t") << std::endl;

    const size_t   M = lib->size;
    vector<double> C0(M,0);
    vector<double> Cs(M,0);

    for(size_t iter=0;iter<8;++iter)
    {
        lib.conc(C0,ran);
        const aftermath am = aftermath::solve(eq,K,C0,Cs,xlim,xmul,xadd);
        const double    Q  = eq.quotient(Cs,K,xmul);
        //std::cerr << "\t\t" << am << std::endl;
        ios::ocstream::echo(AM, "%u %.15g\n",++AI,Q);
    }
}


static inline
void perform(const size_t      nmax,
             randomized::bits &ran)
{

    ios::ocstream::overwrite(AM);

    for(int k=-10;k<=10;++k)
    {
        const double K = pow(10.0,k);
        std::cerr << std::endl;
        std::cerr << "-------- " << K << " --------" << std::endl;

        for(size_t lhs=1;lhs<=nmax;++lhs)
        {
            std::cerr << "lhs=" << lhs << std::endl;
            partition reac(lhs);
            do
            {
                perform(K,&reac,NULL,ran);
                perform(K,NULL,&reac,ran);
            }
            while(reac.next());

            for(size_t rhs=1;rhs<=nmax;++rhs)
            {
                std::cerr << "\tlhs=" << lhs << " / rhs=" << rhs << std::endl;
                partition prod(rhs);

                reac.boot();
                do
                {
                    prod.boot();
                    do
                    {
                        perform(K,&reac,&prod,ran);
                    }
                    while(prod.next());
                }
                while(reac.next());

            }
        }
    }



}


YACK_UTEST(am)
{

    randomized::rand_ ran;
    species::verbose = true;

    perform(5,ran);

    return 0;


    library            lib;
    lua_equilibria     eqs;
    weasel::designer  &wd = weasel::designer::instance();
    cameo::mul<double> xmul;
    cameo::add<double> xadd;
    xlimits            xlim;

    for(int i=1;i<argc;++i)
    {
        wd(lib,eqs,jive::module::open_data(argv[i]));
    }

    std::cerr << "lib=" << std::endl << lib << std::endl;
    std::cerr << "eqs=" << std::endl << eqs << std::endl;

    const size_t   M = lib->size;
    vector<double> C0(M+2,0);
    vector<double> Cs(M,0);
    vector<double> psi(M,0);
    vector<int>    nu(M,0);
    
    lib.conc(C0,ran);
    for(size_t i=M;i>0;--i) if( ran.choice() ) C0[i] = 0;

    lib(std::cerr << "C0=",C0) << std::endl;


    for(enode *en=eqs->head;en;en=en->next)
    {
        equilibrium   &eq = ***en;
        const double   K  = eq.K(0);
        eq.fill(nu,0);
        std::cerr << "nu=" << nu << std::endl;
        std::cerr << "ma=" << eq.mass_action(C0,K,xmul) << std::endl;
        const aftermath am = aftermath::solve(eq,K,C0, Cs,xlim,xmul,xadd);
        std::cerr << "am=" << am << std::endl;
        lib(std::cerr << "Cs=",Cs) << std::endl;
        std::cerr << "ma=" << eq.mass_action(Cs,K,xmul) << std::endl;
        if(am.state == is_running)
        {
            eq.grad(psi,Cs,K,xmul);
            lib(std::cerr << "psi=",psi,"d_") << std::endl;
            const double sigma = xadd.dot(nu,psi);
            std::cerr << "sigma=" << sigma << " / " << eq.slope(Cs,K,xmul,xadd) << std::endl;

            iota::save(C0,Cs);
            const aftermath am2 = aftermath::solve(eq,K,C0,Cs,xlim,xmul,xadd);
            std::cerr << "am2 = " << am2 << std::endl;
        }
    }


}
YACK_UDONE()

