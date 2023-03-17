#include "yack/aqueous/realm/collector.hpp"

namespace yack
{
    namespace aqueous
    {

        survey:: ~survey() noexcept {}

        survey:: survey(const sp_proxy &proxy) noexcept :
        extent(0),
        vanish(proxy),
        request(0),
        nullify(proxy)
        {

        }

        void survey:: initialize() noexcept
        {
            extent  = 0; vanish.clear();
            request = 0; nullify.clear();
        }

        void survey:: on_positive(const double   xi,
                                  const species &sp,
                                  bool          &fp)
        {
            if(fp)
            {
                fp     = false;
                extent =  xi;
                vanish << sp;
            }
            else
            {
                switch( __sign::of(xi,extent) )
                {
                    case negative: // new winner
                        assert(xi<extent);
                        extent = xi;
                        vanish.clear();
                        vanish << sp;
                        break;

                    case __zero__: // ex-aequo
                        vanish << sp;
                        break;

                    case positive: // looser
                        break;
                }

            }
        }

        void survey:: operator()(const actors           &A,
                                 const readable<double> &C)
        {
            initialize();

            bool first_positive = false;
            for(const actor *a = A.head; a; a=a->next)
            {
                const species &s = **a;
                const double   c = C[s.indx[top_level]];
                if(c>=0)
                    on_positive(c/a->nu,s,first_positive);
                else
                {
                }
            }
        }

    }

}


