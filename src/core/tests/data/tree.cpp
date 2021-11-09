#include "yack/data/tree/suffix.hpp"
#include "yack/utest/run.hpp"
#include "yack/apex.hpp"
#include <typeinfo>
#include "yack/ios/icstream.hpp"
using namespace yack;

namespace
{

}

YACK_UTEST(data_tree)
{

    {
        suffix_tree<apq,uint32_t> tree;
        YACK_SIZEOF(tree);
        const char *key  = "hello";
        const apq   value(1/2);

        YACK_CHECK(tree.insert(value,key,1));
        YACK_CHECK(tree.insert(value,key,2));
        YACK_CHECK(tree.insert(value,key,3));
        YACK_CHECK(tree.insert(value,key,4));

        YACK_CHECK(tree.search(key,1));
        YACK_CHECK(tree.search(key,2));
        YACK_CHECK(tree.search(key,3));
        YACK_CHECK(tree.search(key,4));
        YACK_CHECK(!tree.search(key,5));

    }

    if(argc>1)
    {
        suffix_tree<size_t,char> tree;
        ios::icstream   fp(argv[1]);
        ios::characters line;
        size_t          indx = 0;
        size_t          collisions = 0;
        while( fp.gets(line) )
        {
            ++indx;
            char *key = line.cstr();
            try {
               if(!tree.insert(indx,key,line.size))
                   ++collisions;
                line.free(key);
            }
            catch(...)
            {
                line.free(key);
                throw;
            }
        }
        std::cerr << "loaded    =" << tree.size() << std::endl;
        std::cerr << "collisions=" << collisions  << std::endl;

    }




}
YACK_UDONE()

