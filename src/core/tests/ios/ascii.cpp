#include "yack/ios/ocstream.hpp"
#include "yack/ios/ascii/encoded.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

static inline ios::ostream & print_byte(ios::ostream &fp, const unsigned i)
{
    fp("\"x%02X\"",i);
    return fp;
}

static inline ios::ostream & print_text(ios::ostream &fp, const char *txt)
{
    fp << "\"" << txt << "\"";
    return fp;
}

#define CHECK_ENDL() if(0==(count&15)) fp << '\n'

YACK_UTEST(ios_ascii)
{

    {
        ios::ocstream fp("encoded.cxx");
        unsigned count = 0;
        for(unsigned i=0;i<32;++i)
        {
            const char c = char(i);
            switch(c)
            {
                case '\n': print_text(fp,"\\\\n"); break;
                case '\r': print_text(fp,"\\\\r"); break;
                case '\t': print_text(fp,"\\\\t"); break;

                default:
                    print_byte(fp,i);
            }
            fp << ',';
            ++count;
            CHECK_ENDL();
        }

        for(unsigned i=32;i<127;++i)
        {
            const char c = char(i);
            switch(c)
            {
                case '\\': print_text(fp,"\\\\\\\\");; break;
                case '"' : print_text(fp,"\\\\\\\"");; break;
                case '\'': print_text(fp,"\\\\'");; break;

                default:
                    fp("\"%c\"  ",c);
            }
            fp << ',';
            ++count;
            CHECK_ENDL();
        }

        for(unsigned i=127;i<255;++i)
        {
            print_byte(fp,i) << ',';
            ++count;
            CHECK_ENDL();
        }

        print_byte(fp,255) << '\n';
        ++count;
        YACK_CHECK(256==count);
    }

    if(true)
    {
        ios::ocstream fp("ascii.dot");
        fp << "digraph G {\n";
        for(unsigned i=0;i<256;++i)
        {
            fp("_%u [label=\"%s\"];\n",i,ios::ascii::encoded[i]);
        }
        fp << "}\n";
    }


}
YACK_UDONE()


