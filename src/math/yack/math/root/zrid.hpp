
//! \file

#ifndef YACK_ROOT_ZRID_INCLUDED
#define YACK_ROOT_ZRID_INCLUDED 1

#include "yack/math/root/zroot.hpp"
#include <cmath>

namespace yack
{

    namespace math
    {
        namespace core
        {
            extern const char zrid_name[]; //!< "zrid"
        }

        //______________________________________________________________________
        //
        //
        //! Ridder's method
        //
        //______________________________________________________________________
        template <typename T>
        class zrid : public zroot<T>
        {
        public:
            virtual            ~zrid() throw();       //!< setup
            explicit            zrid() throw();       //!< cleanup
            virtual const char *name() const throw(); //!< name
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(zrid);
            
            virtual bool processing(real_function<T>   &F,
                                    triplet<T>         &x,
                                    triplet<T>         &f,
                                    triplet<sign_type> &s) const;

        };

    }

}

#endif

