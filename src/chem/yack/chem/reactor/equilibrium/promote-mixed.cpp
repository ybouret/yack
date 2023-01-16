
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

                inline explicit mixed_equilibrium(const string           &uid,
                                                  const size_t            idx,
                                                  const readable<double> &myK,
                                                  const readable<int>    &arr) :
                equilibrium(uid,idx),
                K_core(myK),
                K_eval(),
                weight(arr,transmogrify),
                zeroed()
                {
                    info = &zeroed;
                }

                virtual ~mixed_equilibrium() throw()
                {
                }
                
                const readable<double> & K_core;
                mutable rmulops          K_eval;
                const cxx_array<int>     weight;
                const sp_repo            zeroed;
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(mixed_equilibrium);
                virtual double getK(double) const
                {
                    K_eval.ld1();
                    for(size_t j=weight.size();j>0;--j)
                    {
                        K_eval.ipower(K_core[j],weight[j]);
                    }
                    return K_eval.query();
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


