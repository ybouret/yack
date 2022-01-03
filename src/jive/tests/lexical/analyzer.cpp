
#include "yack/jive/lexical/analyzer.hpp"
#include "yack/jive/lexical/plugin/single-line-comment.hpp"
#include "yack/jive/lexical/plugin/multi-lines-comment.hpp"

#include "yack/jive/lexical/plugin/jstring.hpp"
#include "yack/jive/lexical/plugin/rstring.hpp"
#include "yack/jive/lexical/plugin/bstring.hpp"
#include "yack/jive/lexical/plugin/verbatim.hpp"

#include "yack/utest/run.hpp"

using namespace yack;

namespace
{



    class mylexer : public jive::lexer
    {
    public:
        explicit mylexer() : jive::lexer("lexer"), t_int(), t_hex()
        {
            verbose = true;
            emit("ID","[:alpha:][:word:]*");
            jump("INT","[:digit:]",this,&mylexer::enterInt);


            endl("[:endl:]");
            drop("[:blank:]+");


            scanner &INT = decl( new scanner("INT") );

            INT.make("[:digit:]+",this,&mylexer::growInt);
            INT.jump("lexer","[:digit:]!",this,&mylexer::leaveInt);

            call("HEXA","0[xX]",this,&mylexer::enterHex);
            scanner &HEXA = decl( new scanner("HEXA") );
            HEXA.make("[:xdigit:]+",this,&mylexer::growHex);
            HEXA.back("[:xdigit:]!",this,&mylexer::leaveHex);


            import( new jive::lexical::multi_lines_comment("C_Comments","/\\*","\\*/",*this) );

            plug( jive::lexical::cxx_comment::clid, "C++Comment");


            plug( type2type<jive::lexical::single_line_comment>(), "Sh_Comment",'#');
            plug( type2type<jive::lexical::multi_lines_comment>(), "XMLComment","<\\!--","-->");

            plug(jive::lexical::verbatim::clid, "block","<verb>","<verb/>");
            plug(jive::lexical::jstring::clid,  "jstring");

            plug(jive::lexical::rstring::clid,  "rstring");
            plug(jive::lexical::bstring::clid,  "bstring");

            
        }

        virtual ~mylexer() throw()
        {
            std::cerr << "~mylexer" << std::endl;
        }
        
        inline void enterInt(jive::token &t)
        {
            std::cerr << "\tenterInt [" << t << "]" << std::endl;
            t_int.swap_with(t);
        }

        inline jive::lexical::behavior growInt(jive::token &t)
        {
            std::cerr << "\tgrowInt [" << t << "]" << std::endl;
            t_int.merge_back(t);
            return yack::jive::lexical::discard;
        }

        inline void leaveInt(jive::token &nxt)
        {
            restore(nxt);
            std::cerr << "\tleaveInt [" << t_int << "]" << std::endl;
            jive::lexeme *lx =  newlex( (*this)["INT"].label, *t_int );
            store(lx);
            (**lx).swap_with(t_int);
        }

        inline void enterHex(jive::token &t)
        {
            std::cerr << "\tenterHex [" << t << "]" << std::endl;
            t_hex.release();
        }
        
        inline jive::lexical::behavior growHex(jive::token &t)
        {
            std::cerr << "\tgrowHex [" << t << "]" << std::endl;
            t_hex.merge_back(t);
            return  jive::lexical::discard;
        }
        
        inline void leaveHex(jive::token &nxt)
        {
            restore(nxt);
            std::cerr << "\tleaveHex [" << t_hex << "]" << std::endl;
            if(t_hex.size<=0) throw exception("missing hex...");
            jive::lexeme *lx = newlex( (*this)["HEXA"].label, *t_hex );
            store(lx);
            (**lx).swap_with(t_hex);
        }
        
        
        jive::token t_int;
        jive::token t_hex;

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(mylexer);
    };
}

YACK_UTEST(analyzer)
{
    YACK_SIZEOF(jive::dictionary);
    YACK_SIZEOF(jive::lexical::scanner);
    YACK_SIZEOF(jive::lexical::analyzer);

    mylexer lex;
    std::cerr << lex.label << " is ready" << std::endl;
    //std::cerr << *lex << std::endl;

    if(false)
    {
        ios::ocstream::overwrite("example.dat");
        ios::ocstream::echo("example.dat","\"string");
    }

    if(argc>1)
    {
        jive::lexemes lxm;
        jive::lexeme *lx = 0;
        {
            jive::source src( jive::module::open_file(argv[1]) );
            while(NULL!=(lx=lex.query(src)))
            {
                lxm.push_back(lx);
                std::cerr << *lx << std::endl;
            }
        }

        for(const jive::lexeme *lx=lxm.head;lx;lx=lx->next)
        {
            const string  s = (**lx).to_string();
            std::cerr << '#' << lx->indx << " : " << *lx <<  " = " << s << std::endl;
        }


    }

}
YACK_UDONE()

