
#include "yack/chem/designer.hpp"

namespace yack
{
    namespace chemical
    {


        const char * const designer:: call_sign = "chemical::designer";

        designer:: ~designer() throw()
        {
        }


        namespace
        {
            class sp_parser : public jive:: parser
            {
            public:
                inline virtual ~sp_parser() throw() {}

                inline explicit sp_parser() : jive:: parser("chemical::species::parser")
                {
                }


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sp_parser);
            };
        }

        designer:: designer() :
        sp( new sp_parser() )
        {
        }

    }

}
