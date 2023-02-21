

#include "yack/jive/pattern/joker/at-least.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/ios/encoder.hpp"

namespace yack
{
    namespace jive
    {
        const char   at_least:: clid[] = "at_least";
        const char * at_least:: class_uid() const noexcept { return clid; }

        at_least:: ~at_least() noexcept
        {
        }

        at_least:: at_least(const size_t n,pattern *p) noexcept :
        joker(mark,p),
        count(n)
        {
            I_am<at_least>();
        }

        bool at_least:: strong() const
        {
            return (count>0) && (**this).strong();
        }

        bool  at_least:: accept(source &src, token &tkn) const
        {
            size_t num = 0;
            while(true)
            {
                token  sub;
                if( (**this).accept(src,sub) )
                {
                    tkn.merge_back(sub);
                    ++num;
                    continue;
                }
                break;
            }
            if(num>=count)
            {
                return true;
            }
            else
            {
                src.store(tkn);
                return false;
            }
        }

        pattern * at_least::create(const size_t n, pattern *p)
        {
            assert(p);
            auto_ptr<pattern> k(p);
            pattern          *j = new at_least(n,p);
            k.dismiss();
            return j;
        }

        size_t at_least:: serialize(ios::ostream &fp) const
        {
            size_t nw = 0;
            switch(count)
            {
                case 0: nw = ios::encoder::emit(fp,zom_); break;
                case 1: nw = ios::encoder::emit(fp,oom_); break;
                default:
                    nw  = emit_uuid(fp);
                    nw += ios::encoder::serialize(fp,count);
                    break;
            }
            return  nw + (**this).serialize(fp);
        }


        pattern * zero_or_more(pattern *p) { return at_least::create(0,p); }
        pattern * one_or_more(pattern *p)  { return at_least::create(1,p); }



        void at_least:: encode(ios::ostream &fp) const
        {
            logo(fp) << "[";
            fp << "label=\"";
            switch(count)
            {
                case 0: fp << '*'; break;
                case 1: fp << '+'; break;
                default:
                    fp(">=%u",unsigned(count));
            }
            fp << "\"";
            fp << ",shape=circle";
            fp << "]";
            end(fp);
            viz(fp);
        }

        at_least:: at_least(const at_least &other) noexcept :
        joker(other),
        count(other.count)
        {
            I_am<at_least>();
        }

        pattern * at_least:: clone() const
        {
            return new at_least(*this);
        }

        bool at_least:: is_univocal() const noexcept
        {
            return false;
        }

    }

}

