
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
            //__________________________________________________________________
            //
            //
            //! primary variable = name+indx
            //
            //__________________________________________________________________
            class primary : public variable
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef ark_ptr<string,const primary> handle; //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit primary(const string &, const size_t); //!< name+indx
                virtual ~primary() throw();                     //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                virtual const string & alias()      const throw(); //!< name
                virtual bool           is_primary() const throw(); //!< true
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(primary);
                virtual size_t indx() const throw(); //!< return indx_
                const   size_t indx_;
            };

        }

    }

}



#endif

