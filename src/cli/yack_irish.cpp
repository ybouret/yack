#include "yack/string.hpp"
#include "yack/program.hpp"
#include "yack/ios/icstream.hpp"
#include "yack/string/ops.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/string/tokenizer.hpp"

using namespace yack;

static inline bool is_space(const int C) throw() { return ' ' == C || '\t' == C; }

YACK_PROGRAM()
{
    ios::icstream   input(ios::cstdin);
    ios::characters line;
    vector<string>  words;
    
    while( input.gets(line) )
    {
        string data = line.to_string();
        strops::clean_if(is_space,data);
        if(data.size()<=0)
        {
            std::cout << std::endl;
            continue;
        }
        
        
        words.free();
        tokenizer::split(words,is_space,data);

        if(words[1]=="Part")
        {
            std::cout << "\\textnote{" << data << "}" << std::endl;
            continue;
        }
        
        std::cout << "\\nolyrics{ \\sep";
        for(size_t i=1;i<=words.size();++i)
        {
            const string &word = words[i];
           // if(word=='|' || word=='-')
            if(word=='|')
            {
                std::cout << " \\sep";
            }
            else
            {
                std::cout << " \\[" << word << "]";
            }
        }
        std::cout << " \\sep }" << std::endl;
        
        
        
    }
    
    
}
YACK_DONE()
