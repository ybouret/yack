
//! \file

#ifndef YACK_FIT_SAMPLE_INCLUDED
#define YACK_FIT_SAMPLE_INCLUDED 1

#include "yack/math/fit/variables.hpp"

namespace yack
{
    namespace math
    {

        namespace fit
        {
            class sample : public object, public counted
            {
            public:

                const string & key() const throw();
                
                const string name;
                variables    vars;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sample);
            };
        }

    }

}

#endif
