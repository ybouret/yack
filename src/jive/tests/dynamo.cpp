
#include "yack/jive/parser.hpp"
#include "yack/utest/run.hpp"

namespace yack
{
    namespace jive
    {

        class dynamo : public parser
        {
        public:
            virtual ~dynamo() noexcept {}

            explicit dynamo() : parser( "dynamo" )
            {
                const rule &END = term(';');

                top( agg("dynamo") << END );
                gv();
                
            }
            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(dynamo);
        };

    }
}

using namespace yack;
using namespace jive;


YACK_UTEST(dynamo)
{
    dynamo dyn;

    if(argc>1)
    {
        const string           fn = argv[1];
        auto_ptr<jive::module> m  = module::open_file(fn);
        std::cerr << m << std::endl;
    }

}
YACK_UDONE()



