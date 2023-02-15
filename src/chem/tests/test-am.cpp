
#include "yack/chem/library.hpp"
#include "yack/chem/eqs/lua.hpp"
#include "yack/chem/aftermath.hpp"

#include "yack/system/env.hpp"
#include "yack/utest/run.hpp"
#include "yack/counting/part.hpp"
#include "yack/math/iota.hpp"

using namespace yack;
using namespace chemical;
using namespace math;

namespace {
    
    enum component_type {
        as_reac,
        as_prod
    };
    
    static inline
    void populate(library                &lib,
                  components             &cmp,
                  const readable<size_t> &cof,
                  unsigned               &idx,
                  const component_type   &cid)
    {
        for(size_t i=1;i<=cof.size();++i)
        {
            const string   name = vformat("a%u",++idx);
            const species &sp   = lib(name,0);
            const size_t   cf   = cof[i];
            switch(cid)
            {
                case as_reac:
                    cmp(sp,-static_cast<int>(cf));
                    break;
                    
                case as_prod:
                    cmp(sp,static_cast<int>(cf));
                    break;
            }
        }
    }
    
    static inline
    void populate(library                &lib,
                  components             &cmp,
                  const readable<size_t> *reac,
                  const readable<size_t> *prod)
    {
        unsigned idx = 0;
        if(reac) populate(lib,cmp,*reac,idx,as_reac);
        if(prod) populate(lib,cmp,*prod,idx,as_prod);
    }
    
    static unsigned I = 0;
    
    static inline void save_init()
    {
        ios::ocstream::overwrite("am.dat");
        ios::ocstream::echo("am.dat", "#idx RQ MA\n");
    }
    
    
    static inline void save(const equilibrium &E,
                            const double       MA,
                            const double       RQ)
    {
        ios::acstream fp("am.dat");
        fp("%u %.15g %.15g # (%.15g) ",++I,RQ,MA,E.K(0));
        fp << E.to_string() << '\n';
    }
    
    static inline
    void perform(const readable<size_t> *reac,
                 const readable<size_t> *prod,
                 randomized::bits       &ran)
    {
        raddops xadd;
        rmulops xmul;
        bool    init = true;
        for(int p=-10;p<=10;++p)
        {
            const_equilibrium E("E",1,pow(10.0,p));
            library           L;
            populate(L,E,reac,prod);
            const double     K  = E.K(0);
            if(init)
            {
                std::cerr << E.content() << " | [";
                init=false;
            }
            
            (std::cerr << ' ' << K).flush();
            
            const size_t      M = L.size();
            vector<double>    Cini(M,0);
            vector<double>    Cend(M,0);
            for(size_t iter=0;iter<10;++iter)
            {
                L.cfill(Cini,ran);
                const aftermath  am = aftermath::guess(E,K, Cini, Cend,xadd,xmul);
                const double     MA = E.mass_action(K,Cend,xmul);
                const double     RQ = E.quotient(K,Cend,xmul);
                save(E,MA,RQ);
            }
            
        }
        std::cerr << " ]" << std::endl;
    }
    
    static inline
    void drive(const size_t      n,
               randomized::bits &ran)
    {
        save_init();
        partition reac(n);
        do
        {
            // special cases
            perform(&reac,NULL,ran);
            perform(NULL,&reac,ran);
            
            partition prod(n);
            do
            {
                perform(&reac,&prod,ran);
            } while(prod.next());
        }
        while(reac.next());
    }
    
    
    
}

YACK_UTEST(am)
{
    entity::verbose = environment::flag("VERBOSE");
    randomized::rand_  ran;
    
    drive(3,ran);
    
    if(true)
    {
        library            lib;
        luaEquilibria      eqs;
        equilibria        &eqs_ = eqs;
        
        for(int i=1;i<argc;++i)
        {
            eqs(lib,argv[i]);
        }
        
        std::cerr << "[[ lib ]]" << std::endl;
        std::cerr << lib << std::endl;
        std::cerr << std::endl;
        
        std::cerr << "[[ eqs ]]" << std::endl;
        std::cerr << eqs_ << std::endl;
        std::cerr << std::endl;
        
        const size_t M = lib.size();
        const size_t N = eqs.size();
        if( N <= 0) return 0;
        
        vector<double> Cini(M,0);
        vector<double> Cend(M,0);
        vector<int>    Nu(M,0);
        
        rmulops xmul;
        raddops xadd;
        for(const enode *en=eqs.head();en;en=en->next)
        {
            const equilibrium &eq = ***en;
            const double       K  = eq.K(0);
            eq.fill(Nu);
            lib.cfill(Cini,ran);
            lib(std::cerr << "Cini=","",Cini);
            std::cerr << "MA_ini: " << eq.mass_action(K,Cini,xmul) << std::endl;
            aftermath am = aftermath::guess(eq,K,Cini,Cend,xadd,xmul);
            std::cerr << am << std::endl;
            lib(std::cerr << "Cend=","",Cend);
            
            const double MA = eq.mass_action(K,Cend,xmul);
            std::cerr << "MA_end: " << MA << std::endl;
            std::cerr << "RQ_end: " << eq.quotient(K,Cend,xmul) << std::endl;
            
        }
    }
 
}
YACK_UDONE()
