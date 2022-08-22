#include "yack/info/compression/alphabet.hpp"
#include "yack/type/out-of-reach.hpp"

#include <cstring>

namespace yack
{
    namespace information
    {

        alphabet:: ~alphabet() throw()
        {
            chars.restart();
            nodes = 0;
            memset(words,0,sizeof(words));
        }

        alphabet:: alphabet() throw() :
        chars(),
        nodes(NULL),
        nyt(NULL),
        end(NULL),
        words()
        {
            nodes =  static_cast<node_t*>( memset(words,0,sizeof(words)) );
            nyt   = &nodes[NYT];
            end   = &nodes[END];

            for(size_t i=0;i<codes;++i)
            {
                node_t &node = nodes[i];
                node.code = i;
                node.info = i;
                node.bits = 8;
                chars.push_back( &node );
            }

            for(size_t i=bytes;i<codes;++i)
            {
                node_t &node = nodes[i];
                node.bits    = 9;
                node.freq    = 1;
            }


        }



    }

}
