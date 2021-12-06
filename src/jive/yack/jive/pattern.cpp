
#include "yack/jive/pattern.hpp"
#include "yack/ios/encoder.hpp"

namespace yack
{
    namespace jive
    {
        bool pattern:: verbose = false;
        
        pattern:: ~pattern() throw()
        {
        }

        pattern:: pattern(const uint32_t t) throw() :
        uuid(t),
        next(0),
        prev(0),
        self(0)
        {

        }

        void pattern:: I_am(const void *addr) throw()
        {
            *(void **)out_of_reach::address(&self) = (void*)(addr);
        }

        size_t pattern:: emit_uuid(ios::ostream &fp) const
        {
            return ios::encoder::emit(fp,uuid);
        }

        bool pattern:: feeble() const
        {
            return !strong();
        }

        bool operator==(const pattern &lhs, const pattern &rhs) throw()
        {
            return pattern::are_same(lhs,rhs);
        }

        bool operator!=(const pattern &lhs, const pattern &rhs) throw()
        {
            return !pattern::are_same(lhs,rhs);
        }

        void pattern:: saveGV(ios::ostream &fp) const
        {
            ios::vizible::digraph_init(fp,"G");
            encode(fp);
            ios::vizible::digraph_quit(fp);
        }

    }

}

#include "yack/ios/ocstream.hpp"

namespace yack
{
    namespace jive
    {

        template <typename FILENAME> static inline
        void doGV(const pattern *p, const FILENAME &filename)
        {
            {
                ios::ocstream fp(filename);
                p->saveGV(fp);
            }

            ios::vizible::render(filename);
        }

        void pattern:: makeGV(const string &filename) const
        {
            doGV(this,filename);
        }

        void pattern:: makeGV(const char *filename) const
        {
            doGV(this,filename);
        }

    }

}
