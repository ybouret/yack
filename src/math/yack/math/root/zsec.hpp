
//! \file

#ifndef YACK_ROOT_ZSEC_INCLUDED
#define YACK_ROOT_ZSEC_INCLUDED 1

#include "yack/math/root/zroot.hpp"
#include <cmath>

namespace yack
{

    namespace math
    {

        namespace core
        {
            extern const char zsec_name[]; //!< "zsec"
        }

        //______________________________________________________________________
        //
        //
        //! secant method
        //
        //______________________________________________________________________
        template <typename T>
        class zsec : public zroot<T>
        {
        public:
            virtual            ~zsec() noexcept;       //!< cleanup
            explicit            zsec() noexcept;       //!< setup
            virtual const char *name() const noexcept; //!< name

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(zsec);

            virtual bool processing(real_function<T>   &F,
                                    triplet<T>         &x,
                                    triplet<T>         &f,
                                    triplet<sign_type> &s) const;
        };

    }

}

#endif

