
#include "yack/concurrent/loop.hpp"

namespace yack
{
    namespace concurrent
    {
        loop:: ~loop() throw()
        {
        }

        loop::loop() throw()
        {
        }


        
        void loop:: run(runnable &obj) throw()
        {
            (*this)(runnable::call,&obj);
        }

    }

}
