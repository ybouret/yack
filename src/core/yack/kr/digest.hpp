#ifndef YACK_CRYPTO_DIGEST_INCLUDED
#define YACK_CRYPTO_DIGEST_INCLUDED 1

#include "yack/sequence/contiguous.hpp"
#include "yack/ios/serializable.hpp"
#include "yack/memory/buffer/rw.hpp"
#include "yack/ios/fwd.hpp"

namespace yack
{
    
    //__________________________________________________________________________
    //
    //
    //! digest interface, fixed length
    //
    //__________________________________________________________________________
    class digest :
    public contiguous<uint8_t>,
    public memory::rw_buffer,
    public ios::serializable
    {
    public:
        static const char clid[]; //!< "digest"
                                  //!
                                  //______________________________________________________________________
                                  //
                                  // C++
                                  //______________________________________________________________________
        digest(const size_t n); //!< setup with n bytes
        digest(const digest &); //!< copy, same size
        digest(const size_t n, const uint8_t b); //!< [b...b] n times
        virtual ~digest() throw(); //!< cleanup
        digest(const char *h);     //!< from hexadecimal string
        
        //______________________________________________________________________
        //
        // serializable
        //______________________________________________________________________
        virtual const char *class_uid() const throw();                //!< clid
        virtual size_t      serialize(ios::ostream &) const;          //!< size+bytes
        static  digest      construct(ios::istream &, size_t &cumul); //!< reload
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        virtual size_t          size()                        const throw(); //!< bytes
        virtual uint8_t       & operator[](const size_t indx)       throw(); //!< [1..size()]
        virtual const uint8_t & operator[](const size_t indx) const throw(); //!< [1..size()]
        virtual uint8_t       * operator*()                         throw(); //!< address
        virtual const uint8_t * operator*()                   const throw(); //!< address
        friend std::ostream   & operator<<(std::ostream  &, const digest &); //!< output
        friend bool operator==(const digest &, const digest &)      throw(); //!< equality
        
        //______________________________________________________________________
        //
        // buffer interface
        //______________________________________________________________________
        virtual size_t          measure() const throw(); //!< bytes
        virtual const void *    ro_addr() const throw(); //!< address of first byte
        
    private:
        YACK_DISABLE_ASSIGN(digest);
        uint8_t     *addr;
        uint8_t     *byte;
        const size_t blen;
        
    };
    
}

#endif

