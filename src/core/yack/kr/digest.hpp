#ifndef YACK_CRYPTO_DIGEST_INCLUDED
#define YACK_CRYPTO_DIGEST_INCLUDED 1

#include "yack/object.hpp"
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
    public object,
    public contiguous<uint8_t>,
    public memory::rw_buffer,
    public ios::serializable
    {
    public:
        static const char clid[]; //!< "digest"

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        digest(const size_t n);                            //!< setup with n bytes
        digest(const digest &);                            //!< copy, same size
        digest(const size_t n, const uint8_t b);           //!< [b...b] n times
        virtual ~digest() throw();                         //!< cleanup
        digest(const char *h);                             //!< from hexadecimal string
        digest(const memory::ro_buffer &);                 //!< from binary data
        digest(const void *, const size_t);                //!< from binary data/text
        digest(const size_t n, const memory::ro_buffer &); //!< setup and fill
        digest & operator=(const digest &) throw();        //!< assign SAME SIZE
                                                           
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
        friend std::ostream   & operator<<(std::ostream  &, const digest &); //!< output
        void                    ldz()                               throw(); //!< set to zero

        //______________________________________________________________________
        //
        // buffer interface
        //______________________________________________________________________
        virtual size_t          measure() const throw(); //!< bytes
        virtual const void *    ro_addr() const throw(); //!< address of first byte
        
        //______________________________________________________________________
        //
        // operators
        //______________________________________________________________________
        digest & operator |= (const digest &rhs) throw();
        digest & operator &= (const digest &rhs) throw();
        digest & operator ^= (const digest &rhs) throw();

        
    private:
        uint8_t     *addr;
        uint8_t     *byte;
        const size_t blen;
        virtual const_type *cxx() const throw();
        virtual const_type *mem() const throw();
    };
    
}

#endif

