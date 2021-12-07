
#include "yack/jive/pattern/regexp/compiler.hpp"
#include "yack/jive/pattern/all.hpp"
#include "yack/jive/pattern/dictionary.hpp"
#include "yack/exception.hpp"
#include "yack/string/tokenizer.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/ios/ascii/convert.hpp"

namespace yack
{
    namespace jive
    {
        //______________________________________________________________________
        //
        // tokenizer separator
        //______________________________________________________________________
        static inline bool is_sep(const char c) throw()
        {
            return c == ',';
        }


        //______________________________________________________________________
        //
        // create a 'NONE' from last pattern
        //______________________________________________________________________
        static inline void exclude_last(logical &p)
        {
            assert(p.size>0);
            op_none *op = new op_none();
            op->push_back( p.pop_back() );
            p.push_back(op);
        }

        //______________________________________________________________________
        //
        // braces with single parameter
        //______________________________________________________________________
        static inline void braces_mono(logical &p, const string &s)
        {
            assert(p.size>0);
            const unsigned count = ios::ascii::convert::to<unsigned>(s,"jive::counting");
            switch(count)
            {
                case 0: exclude_last(p); break;
                case 1: break; // do-nothing!
                default:
                    assert(count>=2);
                    p.push_back( counting::create(count,count,p.pop_back()) );
            }
        }

        //______________________________________________________________________
        //
        // braces with two parameters
        //______________________________________________________________________
        static inline void braces_dual(logical &p, const string &smin, const string &smax)
        {
            assert(p.size>0);
            unsigned nmin = ios::ascii::convert::to<unsigned>(smin,"jive::counting.nmin");
            unsigned nmax = ios::ascii::convert::to<unsigned>(smax,"jive::counting.nmax");
            if(nmin>nmax) cswap(nmin,nmax);
            switch(nmin)
            {
                case 0:
                    switch(nmax)
                    {
                        case  0: exclude_last(p); break;
                        case  1: p.push_back( optional::create(p.pop_back())); break;
                        default:
                            break;
                    }
                    break;

                case 1:
                    if(1==nmax) return; // do nothing
                    break;

                default:
                    assert(nmin>=2);
                    break;

            }
            p.push_back( counting::create(nmin,nmax,p.pop_back()) );
        }

        //______________________________________________________________________
        //
        // check at least one operand
        //______________________________________________________________________
        static inline void check_valid(const logical &p, const string &data)
        {
            if(p.size<=0) throw exception("%s: no expression before '{%s}'", RXCompiler::clid, data());
        }

        //______________________________________________________________________
        //
        // compile braces
        //______________________________________________________________________
        void  RXCompiler:: braces(logical &p)
        {
            //------------------------------------------------------------------
            //
            // detect end of brace
            //
            //------------------------------------------------------------------
            assert(curr[-1]==lbrace);
            const char *org = curr;
            while(curr<last)
            {
                const char c = *(curr++);
                if(rbrace==c)
                {
                    goto FOUND;
                }
            }
            throw exception("%s: unfinished braces in '%s'",clid,expr);

        FOUND:
            //------------------------------------------------------------------
            //
            // extract data
            //
            //------------------------------------------------------------------
            assert(rbrace==curr[-1]);
            const string data(org,(curr-org)-1);
            YACK_JIVE_PRINTLN( RXIndent(deep) << "<braces data='" << data << "'/>");
            if(data.size()<=0) throw exception("%s: empty braces in '%s'",clid,expr);

            const int ch = data[1];
            if(isalpha(ch))
            {
                //--------------------------------------------------------------
                // alias
                //--------------------------------------------------------------
                YACK_JIVE_PRINTLN( RXIndent(deep) << "  -> aliasing" );
                if(!dict) throw exception("%s: no dictionary for '%s'", clid, data());

                const pattern *alias = dict->query(data);
                if(!alias) throw exception("%s: no '%s' in dictionary",clid,data());

                p.push_back(alias->clone());
            }
            else
            {

                if(isdigit(ch))
                {
                    //----------------------------------------------------------
                    // counting
                    //----------------------------------------------------------
                    YACK_JIVE_PRINTLN( RXIndent(deep) << "  -> counting" );
                    vector<string> words(2,as_capacity);
                    switch(tokenizer::split(words,is_sep,data))
                    {
                        case 1: check_valid(p,data); braces_mono(p,words[1]); break;
                        case 2: check_valid(p,data); braces_dual(p,words[1],words[2]); break;
                        default:
                            throw exception("%s: invalid counting braces '{%s}' in '%s'",clid,data(),expr);
                    }
                }
                else
                {
                    //----------------------------------------------------------
                    // failure
                    //----------------------------------------------------------
                    YACK_JIVE_PRINTLN( RXIndent(deep) << "  -> invalid!" );
                    throw exception("%s: invalid braces content '{%s}' in '%s'",clid,data(),expr);
                }
            }


        }
    }

}

