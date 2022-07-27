#include "yack/program.hpp"
#include "yack/ios/pstream.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/string/tokenizer.hpp"
#include "yack/sort/heap.hpp"
#include "yack/string/ops.hpp"

using namespace yack;

YACK_PROGRAM()
{
    if(argc<=1)
    {
        throw exception("usage: %s test-name.exe",program);
    }

    vector<string> cm_list;
    {
        ios::pstream::fill(cm_list,"ctest -N");
    }

    if(cm_list.size()) cm_list.pop_front();
    if(cm_list.size()) cm_list.pop_back();
    if(cm_list.size()) cm_list.pop_back();

    vector<string> words(16,as_capacity);

    for(size_t i=1;i<=cm_list.size();++i)
    {
        string &source = cm_list[i];
        words.free(); tokenizer::split_with(' ',words,source);
        if(!words.size()) throw exception("unexpected empty line in cm_list");
        source.xch( words.back() );
        for(size_t j=source.size();j>0;--j)
        {
            char &c = source[j];
            switch(c)
            {
                case ':':
                case '-':
                    c = '_';
                    break;

                default:
                    break;
            }
        }

    }

    hsort(cm_list,string::compare);
    for(size_t i=1;i<=cm_list.size();++i)
    {
        std::cerr << "#" << std::setw(6) << i << " -> " << cm_list[i] << std::endl;
    }

    vector<string> my_list;
    {
        ios::pstream::fill(my_list,argv[1]);
    }
    if(my_list.size()) my_list.pop_front();
    if(my_list.size()) my_list.pop_back();

    for(size_t i=1;i<=my_list.size();++i)
    {
        string &source = my_list[i];
        strops::strip_with(" \t",2,source);
        if(source.size() && '[' == source.front() ) source.skip(1);
        if(source.size() && ']' == source.back()  ) source.trim(1);
        strops::strip_with(" ",1,source);
    }

    hsort(my_list,string::compare);
    for(size_t i=1;i<=my_list.size();++i)
    {
        std::cerr << "#" << std::setw(6) << i << " -> " << my_list[i] << std::endl;
    }

    const size_t cm_size = cm_list.size();
    const size_t my_size = my_list.size();

    for(size_t i=1;i<=my_size;++i)
    {
        const string &source = my_list[i];
        bool          found  = false;
        for(size_t j=1;j<=cm_size;++j)
        {
            if(source==cm_list[j])
            {
                found = true;
                break;
            }
        }
        if(!found)
        {
            std::cout << "missing '" << source << "'" << std::endl;
        }
    }




}
YACK_DONE()
