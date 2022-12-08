
//! \file

#ifndef YACK_WRITABLE_INCLUDED
#define YACK_WRITABLE_INCLUDED 1

#include "yack/container/readable.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! writable class
    //
    //__________________________________________________________________________
    template <typename T>
    class writable : public readable<T>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef typename readable<T>::type       type;       //!< alias
        typedef typename readable<T>::const_type const_type; //!< alias
        typedef typename readable<T>::param_type param_type; //!< alias

        //______________________________________________________________________
        //
        // interface
        //______________________________________________________________________
        virtual type &operator[](const size_t) throw() = 0; //!< in [1..size]

        //______________________________________________________________________
        //
        // non virtual interface
        //______________________________________________________________________

        //! load same value
        inline void ld(param_type args) {
            for(size_t i=this->size();i>0;--i) (*this)[i] = args;
        }

        //! load increasing
        template <typename U>
        inline void ld_increasing(U value) {
            writable<T> &self = *this;
            const size_t n = self.size();
            for(size_t i=1;i<=n;++i,++value) self[i] = value;
        }

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~writable() throw() {}
    protected:
        inline explicit writable() throw() {}

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(writable);
    };

}

#endif
