
#include "yack/jive/pattern/joker/optional.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    namespace jive
    {

        const char optional:: clid[] = "optional";

        const char * optional:: class_uid() const throw() { return clid; }


        optional:: ~optional() throw() {}


        optional:: optional(pattern *p) throw() : joker(mark,p)
        {
            I_am<optional>();
        }


        bool optional:: strong() const { return false; }

      bool optional:: accept(source &src, token &tkn) const
        {
            assert(0==tkn.size);
            if( (*this).accept(src,tkn) )
            {
                return true;
            }
            else
            {
                assert(0==tkn.size);
                return true;
            }


        }

        pattern * optional::create(pattern *p)
        {
            assert(p);
            auto_ptr<pattern> k(p);
            pattern          *j = new optional(p);
            k.dismiss();
            return j;
        }
        
        size_t optional:: serialize(ios::ostream &fp) const
        {
            const size_t nw = emit_uuid(fp);
            return nw + (**this).serialize(fp);
        }

        void optional:: encode(ios::ostream &fp) const
        {
            logo(fp) << "[";
            fp << "label=\"?\"";
            fp << ",shape=diamond";
            fp << "]";
            end(fp);
            viz(fp);
        }

        optional:: optional(const optional &other) throw() :
        joker(other)
        {
        }
        

        pattern * optional::clone() const
        {
            return new optional(*this);
        }

    }

}

