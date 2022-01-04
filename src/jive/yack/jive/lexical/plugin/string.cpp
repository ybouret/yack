
#include "yack/jive/lexical/plugin/string.hpp"
#include "yack/exception.hpp"
#include "yack/ios/ascii/hybrid.hpp"
#include "yack/ios/fmt/hexa.hpp"
#include "yack/type/hexa.hpp"
#include "yack/string/utf8.hpp"

namespace yack
{
    namespace jive
    {

        namespace lexical
        {
            string_:: ~string_() throw()
            {

            }

            void string_:: setup(const char eos, const char raw)
            {
                back(eos,this, &string_::leave);

                // special symbols
                if(raw!=0)
                {
                    make(raw,this,&string_::claim);
                }
                else
                {
                    make('\"',this,&string_::claim);
                    make('\'',this,&string_::claim);
                }

                // core
                make("[:core:]",this,&string_::claim);

                // escape copy
                make("\\x5c[ \\x5c\\x22\\x27]",this,&string_::esc_copy);

                // escape code
                make("\\x5c[nrtbfv]",this,&string_::esc_code);

                // hexacode
                make("\\x5cx[:xdigit:][:xdigit:]",this,&string_::esc_hexa);

                // utf8 code
                make("\\x5cu[:xdigit:][:xdigit:][:xdigit:][:xdigit:]",this,&string_::esc_utf8);

                // fail esc
                make("\\x5c[\\x00-\\xff]",this,&string_::esc_fail);

                // any one fails now
                make("[\\x00=\\xff]",this,&string_::all_fail);
                
            }

            void string_:: enter(token &t) throw()
            {
                // token is the 'init' single char
                assert(1==t.size);
                content.release();
                content.swap_with(t);
            }

            behavior string_:: claim(token &t) throw()
            {
                content.merge_back(t);
                return discard;
            }

            behavior string_:: esc_copy(token &t) throw()
            {
                assert(2==t.size);
                delete t.pop_front();
                content.merge_back(t);
                return discard;
            }


            behavior string_:: esc_code(token &t) throw()
            {
                assert(2==t.size);
                delete t.pop_front();
                uint8_t &C = coerce(**t.head);
                switch(C)
                {
                    case 'n': C='\n'; break;
                    case 'r': C='\r'; break;
                    case 't': C='\t'; break;
                    case 'b': C='\b'; break;
                    case 'f': C='\f'; break;
                    case 'v': C='\v'; break;
                }
                content.merge_back(t);
                return discard;
            }

            behavior string_:: esc_hexa(token &t) throw()
            {
                assert(4==t.size);
                assert('x'==**(t.head->next));
                delete t.pop(t.head->next);
                const uint8_t lo = hexa::convert(t.pull_back());
                const uint8_t hi = hexa::convert(t.pull_back());

                coerce(t.head->code) = lo + (hi<<4);
                content.merge_back(t);
                return discard;
            }

            behavior string_:: esc_utf8(token &t)
            {
                assert(6==t.size);
                assert('\\'==**(t.head));
                assert('u' ==**(t.head->next));
                // keep context and cleanup
                const context here = *t;
                t.skip(2);
                
                // construct u32
                uint32_t dw = 0;
                for(size_t i=4;i>0;--i)
                {
                    const int h = hexa::convert(t.pull_front());
                    dw <<= 4;
                    dw  |= h;
                }

                //std::cerr << "codepoint=" << ios::hexa(dw,true) << std::endl;
                uint8_t data[4] = { 0,0,0,0 };
                size_t  size    = 0;
                try
                {

                    const utf8 U(dw);      // check utf8
                    size = U.encode(data); // fill  data
                    assert(size<=4);
                }
                catch(exception &excp)
                {
                    here.stamp(excp);
                    throw excp;
                }
                catch(...)
                {
                    throw;
                }

                // modify token
                for(size_t i=0;i<size;++i)
                {
                    content.push_back( new character(here,data[i]) );
                }
                return discard;
            }



            void string_:: leave(token &t)
            {
                // token is the 'done' single char
                assert(1==t.size);

                // merge to finish string
                content.merge_back(t);

                // create a new string, inject into analyzer
                inject(content);
            }

            behavior string_:: esc_fail(token &t)
            {
                assert(2==t.size);
                exception excp("<%s> unexpected of unfinished escaped sequence '\\%s'", (*label)(), ios::ascii::hybrid[**t.tail]);
                (**flux).stamp(excp);
                throw excp;
                return discard;
            }

            behavior string_:: all_fail(token &t)
            {
                assert(1==t.size);
                exception excp("<%s> unexpected '%s'", (*label)(), ios::ascii::hybrid[**t.tail]);
                (**flux).stamp(excp);
                throw excp;
                return discard;
            }

        }

    }

}

