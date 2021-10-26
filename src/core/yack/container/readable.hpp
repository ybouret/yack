//! \file

#ifndef YACK_READABLE_INCLUDED
#define YACK_READABLE_INCLUDED 1

#include "yack/container/collection.hpp"
#include "yack/type/args.hpp"
#include <iostream>

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! readable class
    //
    //__________________________________________________________________________
    template <typename T>
    class readable : public virtual collection
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type); //!< aliases

        //______________________________________________________________________
        //
        // interface
        //______________________________________________________________________
        virtual const_type &operator[](const size_t) const throw() = 0; //!< in [1..size()]

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline friend std::ostream & operator<<(std::ostream &os, const readable<T> &self)
        {
            os << '[';
            const size_t n = self.size();
            if(n>0)
            {
                os << self[1];
                for(size_t i=2;i<=n;++i) os << ';' << self[i];
            }
            os << ']';
            return os;
        }

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~readable() throw() {}
    protected:
        inline explicit readable() throw() {}

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(readable);
    };

}

#endif
