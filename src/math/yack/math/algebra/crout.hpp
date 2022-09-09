
#ifndef YACK_CROUT_INCLUDED
#define YACK_CROUT_INCLUDED 1

#include "yack/math/algebra/crout_.hpp"
#include "yack/math/api.hpp"

#include "yack/memory/operative.hpp"

namespace yack
{
    namespace math
    {

        template <typename T>
        class crout : public crout_
        {
        public:

            YACK_DECL_ARGS_(T,type);
            typedef typename scalar_for<mutable_type>::type scalar_type;
            typedef const scalar_type                       const_scalar_type;
            

            inline virtual ~crout() throw() {}

            inline explicit crout(const size_t dimension) :
            crout_(dimension,sizeof(type),sizeof(scalar_type))
            {
            }
            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(crout);
        };


    }

}

#endif
