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
        suffix_tree<size_t,char>     tree;
        suffix_tree<size_t,uint32_t> tree2;

        ios::icstream   fp(argv[1]);
        ios::characters line;
        size_t          indx = 0;
        size_t          collisions = 0;
        while( fp.gets(line) )
        {
            ++indx;
            char *key = line.cstr();
            try {
                const bool res =tree.insert(indx,key,line.size);
                if(!res)
                    ++collisions;
                YACK_ASSERT(res==tree2.insert(indx,key,line.size));
                line.free(key);
            }
            catch(...)
            {
                line.free(key);
                throw;
            }
        }
        std::cerr << "loaded     = " << tree.size() << std::endl;
        std::cerr << "loaded2    = " << tree2.size() << std::endl;
        std::cerr << "collisions = " << collisions  << std::endl;

        std::cerr << "  <output>" << std::endl;
        suffix_tree<size_t,char>::vkey_type vkey;
        for(const suffix_tree<size_t,char>::knot_type *knot=tree.head();knot;knot=knot->next)
        {
            std::cerr << "data=" << **knot << std::endl;
            knot->node->print_key();
            knot->node->save(vkey);
            std::cerr << "    |_" << vkey << std::endl;
        }
        std::cerr << "  <output/>" << std::endl;

        tree.free();
        std::cerr << "loaded     = " << tree.size() << std::endl;

        if(tree2.size()<=10)
        {
            std::cerr << "  <output2>" << std::endl;
            suffix_tree<size_t,uint32_t>::vkey_type vkey2;
            for(const suffix_tree<size_t,uint32_t>::knot_type *knot=tree2.head();knot;knot=knot->next)
            {
                std::cerr << "data=" << **knot << std::endl;
                knot->node->print_key();
                knot->node->save(vkey2);
                std::cerr << "    |_" << vkey2 << std::endl;
            }
            std::cerr << "  <output2/>" << std::endl;
        }

        tree2.release();
    }




}
YACK_UDONE()

