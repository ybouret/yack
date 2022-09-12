
//! \file

#ifndef YACK_FITTING_PRIMARY_INCLUDED
#define YACK_FITTING_PRIMARY_INCLUDED 1


#include "yack/math/fitting/variable.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {

            class primary : public variable
            {
            public:
                typedef ark_ptr<string,const primary> handle;

                explicit primary(const string &, const size_t);
                virtual ~primary() throw();

                virtual const string & alias()      const throw();
                virtual bool           is_primary() const throw();
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(primary);
                virtual size_t indx() const throw(); //!< return indx_
                const   size_t indx_;
            };

        }

    }

}



#endif

