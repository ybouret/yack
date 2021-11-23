#include "yack/data/suffix/node.hpp"
#include "yack/ios/ascii/hybrid.hpp"

namespace yack
{
    namespace kernel
    {
        tree_node_:: tree_node_() throw()
        {
        }

        tree_node_:: ~tree_node_() throw()
        {
        }

        template <typename OSTREAM> static inline
        void tree_node_format(OSTREAM &os, const void *addr, size_t size)
        {
            assert(!(NULL==addr&&size>0));
            const uint8_t *p = static_cast<const uint8_t *>(addr);
            while(size-- > 0)
            {
                os << ios::ascii::hybrid[*(p++)];
            }
        }

        void tree_node_:: format(std::ostream &os, const void *addr, const size_t size)
        {
            tree_node_format(os,addr,size);
        }

        void tree_node_:: format(ios::ostream &os, const void *addr, const size_t size)
        {
            tree_node_format(os,addr,size);
        }
    }
}
