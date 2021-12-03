
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
        patterns()
        {
        }

        logical & logical:: operator<<( pattern *p ) throw()
        {
            assert(p);
            push_back(p);
            return *this;
        }

        size_t logical:: serialize(ios::ostream &fp) const
        {
            size_t nw = emit_uuid(fp);
            nw       += ios::encoder::serialize(fp,size);
            for(const pattern *m=head;m;m=m->next)
            {
                nw += m->serialize(fp);
            }
            return nw;
        }

        void logical::viz(ios::ostream &fp) const
        {
            unsigned indx=1;
            for(const pattern *m=head;m;m=m->next,++indx)
            {
                m->encode(fp);
                link(fp,m);
                if(size>1)
                {
                    fp("[label=\"%u\"]",indx);
                }
                end(fp);
            }
        }
        

    }

}

#include "yack/jive/pattern/basic/single.hpp"
#include "yack/jive/pattern/basic/within.hpp"

namespace yack
{
    namespace jive
    {
        logical & logical:: add(const uint8_t c)
        {
            return (*this) << new single(c);
        }

        logical & logical:: add(const uint8_t a,const uint8_t b)
        {
            return (*this) << new within(a,b);
        }


        pattern * logical::fill(logical *p, const char *buff, size_t size)
        {
            assert(p);
            assert(yack_good(buff,size));
            
            auto_ptr<pattern> q(p);
            while(size-- > 0)
            {
                (*p) << new single( *(buff++) );
            }

            return optimize(q.yield());
        }
    }
}

