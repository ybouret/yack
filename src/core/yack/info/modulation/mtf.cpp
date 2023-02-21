#include "yack/info/modulation/mtf.hpp"
#include <cstring>

namespace yack
{

    namespace information
    {
        mtf:: ~mtf() noexcept
        {
            nodes.restart();
            memset(store,0,sizeof(store));
        }

        mtf:: mtf() noexcept : nodes(), store()
        {
            reset();
        }

        void mtf:: reset() noexcept
        {
            nodes.restart();
            node_t *node = store;
            for(size_t i=0;i<256;++i)
            {
                node->next = 0;
                node->prev = 0;
                node->data = uint8_t(i);
                nodes.push_back(node);
                ++node;
            }
            assert(256==nodes.size);
        }

        
    }

}


namespace yack
{

    namespace information
    {
        mtf_encoder:: mtf_encoder() noexcept
        {
        }

        mtf_encoder:: ~mtf_encoder() noexcept
        {
        }
        
        uint8_t mtf_encoder:: alter(const uint8_t x) noexcept
        {
            assert(256==nodes.size);
            node_t *node = nodes.head;
            uint8_t indx = 0;
            while(x!=node->data)
            {
                ++indx;
                node=node->next;
            }
            (void)nodes.move_to_front(node);
            return indx;
        }

    }

}

namespace yack
{

    namespace information
    {
        mtf_decoder:: mtf_decoder() noexcept
        {
        }

        mtf_decoder:: ~mtf_decoder() noexcept
        {
        }

        uint8_t mtf_decoder:: alter(const uint8_t x) noexcept
        {
            assert(256==nodes.size);
            node_t *node = nodes.head;
            {
                size_t  step = x;
                while(step-- > 0) node=node->next;
            }
            return nodes.move_to_front(node)->data;
        }

    }

}
