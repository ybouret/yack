#include "yack/chem/com/finalizable.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace Chemical
    {
        Finalizable:: Finalizable() noexcept : finalized(false) {}

        Finalizable:: ~Finalizable() noexcept
        {


        }

        void Finalizable::finalize(const char * const clid)
        {
            assert(clid);
            if(finalized) throw imported::exception(clid,"arleady finalized!");
            doFinalize();
            coerce(finalized) = true;
        }


    }
}
