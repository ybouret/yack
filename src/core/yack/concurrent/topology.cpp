#include "yack/concurrent/topology.hpp"
#include "yack/type/utils.hpp"
#include "yack/system/hw.hpp"
#include "yack/system/env.hpp"
#include "yack/string/tokenizer.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/string/ops.hpp"
#include "yack/exception.hpp"
#include "yack/ios/ascii/convert.hpp"

namespace yack
{

    namespace concurrent
    {

        namespace quark
        {
            topology:: ~topology() throw() {}


            void topology:: linear(const size_t n)
            {
                for(size_t i=0;i<n;++i)  add(i);
            }

            topology:: topology(const size_t n) :
            ulist_type()
            {
                linear( max_of<size_t>(1,n) );

            }




            static inline size_t get_count(const string &word)
            {
                static const char fn[] ="topology.count";
                const size_t n = ios::ascii::convert::to<size_t>(word,fn);
                if(n<=0) throw exception("%s: invalid %u",fn,unsigned(n));
                return n;
            }

            static inline size_t get_shift(const string &word)
            {
                static const char fn[] ="topology.shift";
                return ios::ascii::convert::to<size_t>(word,fn);
            }

            static inline size_t get_every(const string &word)
            {
                static const char fn[] ="topology.every";
                const size_t n = ios::ascii::convert::to<size_t>(word,fn);
                if(n<=0) throw exception("%s: invalid %u",fn,unsigned(n));
                return n;
            }


            static inline void fill_topology(ulist_type  &topo,
                                             const size_t count,
                                             const size_t shift,
                                             const size_t every)
            {
                size_t indx = shift;
                for(size_t i=count;i>0;--i,indx+=every)
                {
                    topo.add(indx);
                }
            }


            void topology:: expand(const string &content)
            {
                static const char fn[]  = "concurrent::topology";

                vector<string> words;
                tokenizer::split_with(':', words, content);
                const size_t nw = words.size();
                for(size_t i=nw;i>0;--i) strops::clean_with(" \t",2,words[i]);

                switch(nw)
                {
                    case 1: fill_topology(*this,get_count(words[1]),0,1); break;
                    case 2: fill_topology(*this,get_count(words[1]),get_shift(words[2]),1); break;
                    case 3: fill_topology(*this,get_count(words[1]),get_shift(words[2]),get_every(words[3])); break;
                    default:
                        throw exception("%s invalid #fields=%u",fn, unsigned(nw));
                }
            }


            topology:: topology() : ulist_type()
            {
                string content;
                if( environment::get("YACK_NUM_THREADS", &content) )
                {
                    // process
                    expand(content);
                }
                else
                {
                    // default: all procs
                    linear( hardware::nprocs() );
                }
            }

            topology:: topology(const string &content) : ulist_type()
            {
                expand(content);
            }

            topology:: topology(const char *content) : ulist_type()
            {
                const string _(content);
                expand(_);
            }


            std::ostream & operator<<(std::ostream &os, const topology &topo)
            {
                os << '[';
                const unode_type *node = topo.head;
                if(node)
                {
                    os << **node;
                    for(node=node->next;node;node=node->next)
                    {
                        os << ';' << **node;
                    }
                }
                os << ']';
                return os;
            }

        }

    }

}

