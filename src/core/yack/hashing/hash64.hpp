//! \file

#ifndef YACK_HASH64_INCLUDED
#define YACK_HASH64_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace hashing
    {
        //______________________________________________________________________
        //
        //
        //! hashing 64 bits
        //
        //______________________________________________________________________
        class hash64
        {
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
        protected:
            explicit hash64() noexcept; //!< setu[
        public:
            virtual ~hash64() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual const char *protocol()                                   const noexcept = 0; //!< name
            virtual void        operator()(uint32_t &lword, uint32_t &rword) const noexcept = 0; //!< alog

            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________

            //! mix value content
            template <typename T> inline
            T mix(const T value) const noexcept
            {
                union {
                    uint32_t dw[2];
                    T        xx;
                } alias = { {0,0} };
                alias.xx = value;
                (*this)(alias.dw[0],alias.dw[1]);
                return alias.xx;
            }

            //! partial value content
            template <typename T,typename U> inline
            U cut(const T value) const noexcept
            {
                union {
                    uint32_t dw[2];
                    T        xx;
                    U        yy;
                } alias = { {0,0} };
                alias.xx = value;
                (*this)(alias.dw[0],alias.dw[1]);
                return alias.yy;
            }



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(hash64);
        };

    }
}

#endif

