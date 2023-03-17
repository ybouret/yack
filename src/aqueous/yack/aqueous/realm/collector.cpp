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

        void survey:: reset() noexcept
        {
            extent  = 0; vanish.clear();
            request = 0; nullify.clear();
        }

        void survey:: on_positive(const double   xi,
                                  const species &sp,
                                  bool          &fp)
        {
            std::cerr << "on_positive " << sp << std::endl;
            if(fp)
            {
                fp     = false;
                extent =  xi;
                vanish << sp;
                assert(vanish.size>0);
            }
            else
            {
                assert(vanish.size>0);
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
                assert(vanish.size>0);
            }
        }

        void survey:: probe(const actors           &A,
                            const readable<double> &C)
        {
            reset();

            bool first_positive = true;
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


