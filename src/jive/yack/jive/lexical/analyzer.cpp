
#include "yack/jive/lexical/analyzer.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {

        namespace lexical
        {

            analyzer:: ~analyzer() throw()
            {
                sdb.release();
                assert(quantity()>0);
                liberate();
            }

            const analyzer::scan_set & analyzer:: operator*() const throw()
            {
                return sdb;
            }


            void   analyzer:: setup()
            {
                withhold();
                const scan_ptr tmp = this;
                if(!sdb.insert(tmp)) throw exception("<%s> unexpected self insertion", (*label)());
            }


        }

    }

}

