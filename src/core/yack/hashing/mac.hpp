
//! \file

#ifndef YACK_HASHING_MAC_INCLUDED
#define YACK_HASHING_MAC_INCLUDED 1


#include "yack/hashing/hmac.hpp"
#include "yack/ptr/str.hpp"

namespace yack
{
    namespace hashing
    {

#define YACK_HMAC_ON_PROLOG() \
function( FUNCTION::__length, FUNCTION::__window), H(), M(H,

#define YACK_HMAC_ON_EPILOG() \
), id(hmac::prefix,H.name())

        template <typename FUNCTION>
        class hmac_on : public function
        {
        public:
            inline explicit hmac_on(const void *key_addr, const size_t key_size) :
            YACK_HMAC_ON_PROLOG() key_addr,key_size YACK_HMAC_ON_EPILOG() {}

            inline explicit hmac_on(const memory::ro_buffer &usr) :
            YACK_HMAC_ON_PROLOG() usr  YACK_HMAC_ON_EPILOG() {}

            inline virtual ~hmac_on() throw() {}

            inline virtual void set() throw() { M.set(H); }
            inline virtual void run(const void *block_addr, size_t block_size) throw()
            {
                H.run(block_addr,block_size);
            }

            inline virtual void get(void *output, const size_t outlen) throw()
            {
                const memory::ro_buffer &md = M.get(H);
                fill(output,outlen,md.ro_addr(),md.measure());
            }

            virtual const char *name() const throw() { return &id[0]; }

            FUNCTION H;
            hmac     M;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(hmac_on);
            const cstr_ptr id;
        };

    }

}

#endif
