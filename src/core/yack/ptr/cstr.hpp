
//! \file

#ifndef YACK_STR_PTR_INCLUDED
#define YACK_STR_PTR_INCLUDED 1

#include "yack/memory/buffer/ro.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! specific ptr for local C-strings
    //
    //__________________________________________________________________________
    class cstr_ptr : public memory::ro_buffer
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        cstr_ptr(const size_t);      //!< allocate
        cstr_ptr(const char *);      //!< copy text
        cstr_ptr(const char *,const char *); //!< merge text
        cstr_ptr(const cstr_ptr &);  //!< copy data
        virtual ~cstr_ptr() throw(); //!< cleanup

        //______________________________________________________________________
        //
        // buffer interface
        //______________________________________________________________________
        virtual const void *ro_addr() const throw(); //!< first char address
        virtual size_t      measure() const throw(); //!< length

        //______________________________________________________________________
        //
        // access
        //______________________________________________________________________
        char       & operator[](const size_t indx)       throw(); //!< [0..length-1]
        const char & operator[](const size_t indx) const throw(); //!< [0..length]
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        //! display
        template <typename OSTREAM> inline
        friend OSTREAM & operator<<(OSTREAM &os, const cstr_ptr &p)
        {
            const size_t n = p.length;
            for(size_t i=0;i<n;++i) os << p[i];
            return os;
        }

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const size_t length; //!< initial and fixed length

    private:
        YACK_DISABLE_ASSIGN(cstr_ptr);
        const size_t in_mem;
        char        *ch;
    };

}

#endif
