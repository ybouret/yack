
#include "yack/jive/parser.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace
{
    class myparser : public jive::parser
    {
    public:
        explicit myparser() : jive::parser("JSON")
        {
        }
        
        virtual ~myparser() throw()
        {
        }
        
    
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(myparser);
    };
}

YACK_UTEST(parser)
{
    myparser J;
    std::cerr << J.label << std::endl;
    
}
YACK_UDONE()


