
//! \file

#ifndef YACK_FIT_VARIABLES_INCLUDED
#define YACK_FIT_VARIABLES_INCLUDED 1

#include "yack/math/fit/replica.hpp"
#include "yack/associative/suffix/set.hpp"

namespace yack
{
    namespace math
    {

        namespace fit
        {

            typedef suffix_set<string,replica::handle> variables_;

            class variables : public variables_
            {
            public:
                typedef replica::handle handle;
                
                explicit variables() throw();
                virtual ~variables() throw();
                variables(const variables &);
                variables & operator=(const variables &);

                template <typename ID>
                variables & operator<<(const ID &id) {
                    const handle h = new primary(id,size()+1);
                    grow(h);
                    return *this;
                }

            private:
                void grow(const handle &);

            };



        }

    }
}

#endif

