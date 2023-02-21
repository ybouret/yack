//! \file

#ifndef YACK_ROOT_ZBIS_INCLUDED
#define YACK_ROOT_ZBIS_INCLUDED 1

#include "yack/math/root/zroot.hpp"
#include <cmath>

namespace yack
{
    
    namespace math
    {

        namespace core
        {
            extern const char zbis_name[]; //!< "zbis"
        }

        //______________________________________________________________________
        //
        //
        //! bisection method
        //
        //______________________________________________________________________
        template <typename T>
        class zbis : public zroot<T>
        {
        public:
            virtual            ~zbis() noexcept;       //!< cleanup
            explicit            zbis() noexcept;       //!< setup
            virtual const char *name() const noexcept; //!< name

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(zbis);

            virtual bool processing(real_function<T>   &F,
                                    triplet<T>         &x,
                                    triplet<T>         &f,
                                    triplet<sign_type> &s) const;
        };
        
    }
    
}

#endif

