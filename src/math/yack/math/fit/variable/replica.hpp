

//! \file

#ifndef YACK_FIT_REPLICA_INCLUDED
#define YACK_FIT_REPLICA_INCLUDED 1

#include "yack/math/fit/variable/primary.hpp"
#include "yack/ptr/ark.hpp"

namespace yack
{
    namespace math
    {

        namespace fit
        {
            //__________________________________________________________________
            //
            //
            //! replica variable, linked to a primary
            //
            //__________________________________________________________________
            class replica : public variable
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef ark_ptr<string,const variable> handle; //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup
                virtual ~replica() throw();

                //! setup
                template <typename ID>
                explicit replica(const ID &i,const handle &h) :
                variable(i),
                original(h)
                {

                }

                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(replica);
                virtual size_t get_index() const throw();
                const handle original;
            };
        }
    }
}

#endif


