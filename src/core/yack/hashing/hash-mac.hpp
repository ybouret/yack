//! \file
#ifndef YACK_HASHING_HASH_MAC_INCLUDED
#define YACK_HASHING_HASH_MAC_INCLUDED 1

 
#include "yack/kr/digest.hpp"
#include "yack/hashing/function.hpp"

namespace yack
{
    namespace hashing
    {

        //! computing a Hash MAC from a hasing function
        class hash_mac
        {
        public:
            //! setup
            explicit hash_mac(function &H, const memory::ro_buffer &key );
            
            
            //! cleanup
            virtual ~hash_mac() throw();

            void set( function &H ) throw(); //!< initialize
            void get( function &H, void *output, size_t outlen ) throw(); //!< finalize

        private:
            const     size_t L;
            const     size_t B;
            digest    key_;
            digest    tmp_;
            digest    end_;
            YACK_DISABLE_COPY_AND_ASSIGN(hash_mac);
        };

        
    }
    
}


#if 0
        //! embedded HMAC function
        template <typename HFN>
        class hmac : public function
        {
        public:
            //! setup
            explicit hmac( const memory::ro_buffer &key ) :
            function( HFN:: __length, HFN:: __window ),
            hfn_(),
            mac_( hfn_, key ),
            name_( string("HMAC-") + hfn_.name() )
            {
            }

            //! destructor
            virtual ~hmac() throw() {}

            //! initialize
            virtual void set() throw() { mac_.set( hfn_ ); }

            //! propagate
            virtual void run( const void *buffer, size_t buflen ) throw() { hfn_.run(buffer,buflen); }

            //! finalize
            virtual void get( void *output, size_t outlen ) throw() { mac_.get( hfn_, output, outlen); }

            //! compound name
            virtual const char *name() const throw() { return *name_; }

        private:
            HFN          hfn_;
            hash_mac     mac_;
            const string name_;
            Y_DISABLE_COPY_AND_ASSIGN(hmac);
        };

    }


}
#endif

#endif
