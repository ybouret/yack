
#include "yack/chem/reactor/cluster.hpp"
#include "yack/sequence/cxx-array.hpp"

namespace yack
{
    
    namespace chemical
    {
        namespace
        {
            class mixed_equilibrium : public equilibrium
            {
            public:
                virtual ~mixed_equilibrium() throw()
                {
                }
                
                const readable<double> &Ktab;
                mutable rmulops         xmul;
                const cxx_array<int>    coef;
                
                explicit mixed_equilibrium(const string           &uid,
                                           const size_t            idx,
                                           const readable<double> &myK,
                                           const readable<int>    &arr) :
                equilibrium(uid,idx),
                Ktab(myK),
                xmul(),
                coef(arr,transmogrify)
                {
                    
                }
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(mixed_equilibrium);
                virtual double getK(double) const
                {
                    xmul.ld1();
                    for(size_t j=coef.size();j>0;--j)
                    {
                        xmul.ipower(Ktab[j],coef[j]);
                    }
                    return xmul.query();
                }
            };
        }
        
        
        const equilibrium &cluster:: promote_mixed(const readable<int>    &weight,
                                                   const readable<int>    &stoich,
                                                   const readable<double> &K,
                                                   const library          &lib,
                                                   const equilibria       &eqs,
                                                   equilibria             &all)
        {


            //----------------------------------------------------------
            // create a mixed equilibrium
            //----------------------------------------------------------
            const string name   = eqs.make_name(weight);
            const size_t mxid   = all.size()+1;
            equilibrium &mxeq   = all.use( new mixed_equilibrium(name,mxid,K,weight) );


            const size_t M = stoich.size();
            for(size_t j=1;j<=M;++j)
            {
                const int f = stoich[j];
                if(f) mxeq( lib[j], f);
            }

            assert(mxeq.neutral());

            //----------------------------------------------------------
            // to register in this related group
            //----------------------------------------------------------
            return mxeq;
        }
        
    }
    
}


