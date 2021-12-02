
#include "yack/jive/pattern/logic/logical.hpp"
#include "yack/ios/encoder.hpp"

namespace yack
{
    namespace jive
    {
        logical:: ~logical() throw()
        {
        }

        logical:: logical(const uint32_t t) throw() :
        pattern(t),
        motifs()
        {
        }

        logical & logical:: operator<<( pattern *p )
        {
            push_back( motif::make(p) );
            return *this;
        }

        size_t logical:: serialize(ios::ostream &fp) const
        {
            size_t nw = emit_uuid(fp);
            nw       += ios::encoder::serialize(fp,size);
            for(const motif *m=head;m;m=m->next)
            {
                nw += (**m).serialize(fp);
            }
            return nw;
        }

        

    }

}
