
#include "yack/chem/realm/engine.hpp"

namespace yack
{
    namespace chemical
    {
        engine:: ~engine() noexcept
        {
        }

        engine:: engine(const realm &cs) :
        engine_()
        {
            for(const domain *dom = cs.head; dom; dom=dom->next)
            {
                push_back( new reactor(*dom) );
            }
        }

        void engine:: operator()(const xmlog            &xml,
                                 writable<double>       &C0,
                                 const readable<double> &K)
        {
            YACK_XMLSUB(xml,"engine");
            for(reactor *reac=head;reac;reac=reac->next)
            {
                reac->solve(xml,C0,K);
            }
        }
    }

}


