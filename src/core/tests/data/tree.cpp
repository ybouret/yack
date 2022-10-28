#include "yack/data/suffix/tree.hpp"
#include "yack/utest/run.hpp"
#include "yack/apex.hpp"
#include <typeinfo>
#include "yack/ios/icstream.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/data/small/list.hpp"

using namespace yack;

namespace
{

}

YACK_UTEST(data_tree)
{

    randomized::rand_ ran;

    {
        kernel::suffix_tree<apq,char> tree;
        YACK_SIZEOF(tree);
        const char *key  = "hello";
        const apq   value(1/2);

        YACK_CHECK(tree.insert(value,key,1));
        YACK_CHECK(tree.insert(value,key,2));
        YACK_CHECK(tree.insert(value,key,3));
        YACK_CHECK(tree.insert(value,key,4));

        {
            tree.gv("tree.dot");
            ios::vizible::render("tree.dot");
        }

        std::cerr << "tree.depth=" << tree.depth() << std::endl;

        YACK_CHECK(tree.search(key,1));
        YACK_CHECK(tree.search(key,2));
        YACK_CHECK(tree.search(key,3));
        YACK_CHECK(tree.search(key,4));
        YACK_CHECK(!tree.search(key,5));

        YACK_CHECK(!tree.remove(key,5));
        YACK_CHECK(tree.remove(key,2));
        YACK_CHECK(!tree.remove(key,2));
        YACK_CHECK(tree.remove(key,1));
        YACK_CHECK(!tree.remove(key,1));
        YACK_CHECK(tree.remove(key,3));
        YACK_CHECK(!tree.remove(key,3));
        YACK_CHECK(tree.remove(key,4));
        YACK_CHECK(!tree.remove(key,4));

        YACK_CHECK( (*tree).size==0);

    }


    if(argc>1)
    {
        kernel::suffix_tree<size_t,char>     tree;
        typedef kernel::tree_path<char>      key_type;
        small_list<key_type>         keys;

        ios::icstream   fp(argv[1]);
        ios::characters line;
        size_t          indx = 0;
        size_t          collisions = 0;
        while( fp.gets(line) )
        {
            ++indx;
            char *path = line.cstr();
            try {
                const bool res = tree.insert(indx,path,line.size);
                if(!res)
                    ++collisions;
                else
                {
                    {
                        key_type k;
                        (*tree).tail->node->encode(k);
                        keys.add(k);
                    }
                    std::cerr << "+" << keys.back() << std::endl;
                }
                line.free(path);
            }
            catch(...)
            {
                line.free(path);
                throw;
            }
        }

        tree.gv("stree.dot");
        ios::vizible::render("stree.dot");
        std::cerr << "tree.depth = " << tree.depth() << std::endl;
        std::cerr << "collisions = " << collisions   << std::endl;

        randomized::shuffle::list(keys,ran);
        for( small_node<key_type> *k = keys.head; k; k=k->next )
        {
            const key_type &key = **k;
            std::cerr << key << std::endl;
        }

        const size_t half = keys.size/2;
        while(keys.size>half)
        {
            const key_type &key = **keys.tail;
            std::cerr << "Removing " << key << std::endl;
            YACK_ASSERT(tree.remove(&key[1],key.size()));
            keys.trim();
        }
        tree.gv("shalf.dot");
        ios::vizible::render("shalf.dot");

        {
            std::cerr << "Making a copy..." << std::endl;
            kernel::suffix_tree<size_t,char> tree_copy(tree);
        }

    }


    




}
YACK_UDONE()

