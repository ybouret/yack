
#include "yack/chem/reactor.hpp"

namespace yack
{
    namespace chemical
    {

        void reactor:: compile_claws(const xmlog &xml)
        {
            for(const cluster *cc=linked->head;cc;cc=cc->next)
                coerce(Nq) += cc->canon->size;

            YACK_XMLOG(xml, "-- conservations:");
            YACK_XMLOG(xml, "Nq = " << Nq);

            if(Nq)
            {
                matrix<unsigned>  &Q = coerce(Qm); Q.make(Nq,M);
                size_t             i = 0;
                for(const cluster *cc=linked->head;cc;cc=cc->next)
                {
                    for(const claw *cl = cc->canon->head;cl;cl=cl->next)
                    {
                        writable<unsigned> &q = Q[++i];
                        for(const actor *a = (*cl)->head; a; a=a->next)
                        {
                            q[***a] = a->nu;
                        }
                    }
                }
                YACK_XMLOG(xml, "Qm = " << Qm);
            }
        }
    }

}


