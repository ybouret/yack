
//! \file

#ifndef YACK_HASHING_MAC_INCLUDED
#define YACK_HASHING_MAC_INCLUDED 1


#include "yack/hashing/hmac.hpp"
#include "yack/ptr/cstr.hpp"

namespace yack
{
    namespace hashing
    {
        //! helper for constructor
#define YACK_HMAC_ON_PROLOG() \
function( FUNCTION::__length, FUNCTION::__window), H(), M(H,

        //! helper for constructor
#define YACK_HMAC_ON_EPILOG() \
), id(hmac::prefix,H.name())
        
        //______________________________________________________________________
        //
        //
        //! assembling HMAC function
        //
        //______________________________________________________________________
        template <typename FUNCTION>
        class hmac_on : public function
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            inline explicit hmac_on(const void *key_addr, const size_t key_size) :
            YACK_HMAC_ON_PROLOG() key_addr,key_size YACK_HMAC_ON_EPILOG() {}

            //! setup
            inline explicit hmac_on(const memory::ro_buffer &usr) :
            YACK_HMAC_ON_PROLOG() usr  YACK_HMAC_ON_EPILOG() {}

            //! cleanup
            inline virtual ~hmac_on() noexcept {}

            //__________________________________________________________________
            //
            // function interface
            //__________________________________________________________________

            //! prepare function
            inline virtual void set() noexcept { M.set(H); }

            //! run function
            inline virtual void run(const void *block_addr, size_t block_size) noexcept
            {
                H.run(block_addr,block_size);
            }

            //! get function from hmac
            inline virtual void get(void *output, const size_t outlen) noexcept
            {
                const memory::ro_buffer &md = M.get(H);
                fill(output,outlen,md.ro_addr(),md.measure());
            }

            //! compound name
            virtual const char *name() const noexcept { return &id[0]; }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            FUNCTION H; //!< base hashing function
            hmac     M; //!< base hash mac creator
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(hmac_on);
            const cstr_ptr id;
        };

    }

}

#endif
