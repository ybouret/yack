
#include "yack/jive/lexical/analyzer.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace
{



    class mylexer : public jive::lexer
    {
    public:
        explicit mylexer() : jive::lexer("lexer"), t_int()
        {
            verbose = true;
            emit("ID","[:alpha:][:word:]*");
            jump("getInt","[:digit:]",this,&mylexer::enterInt);


            endl("[:endl:]");
            drop("[:blank:]+");

            scanner &getInt = decl( new scanner("getInt") );
            getInt.make("[:digit:]+",this,&mylexer::growInt);
            getInt.jump("lexer","[:digit:]!",this,&mylexer::leaveInt);


        }

        virtual ~mylexer() throw()
        {
        }
        
        inline void enterInt(jive::token &t)
        {
            std::cerr << "enterInt [" << t << "]" << std::endl;
            t_int.swap_with(t);
        }

        inline jive::lexical::behavior growInt(jive::token &t)
        {
            std::cerr << "growInt [" << t << "]" << std::endl;
            t_int.merge_back(t);
            return yack::jive::lexical::discard;
        }

        inline void leaveInt(jive::token &nxt)
        {
            restore(nxt);
            std::cerr << "leaveInt [" << t_int << "]" << std::endl;
            jive::lexeme *lx =  new jive::lexeme( (*this)["getInt"].label, *t_int );
            store(lx);
            (**lx).swap_with(t_int);
        }

        jive::token t_int;

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
    std::cerr << *lex << std::endl;

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
            std::cerr << *lx << std::endl;
        }


    }

}
YACK_UDONE()

