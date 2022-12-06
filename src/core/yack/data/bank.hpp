//! \file

#ifndef YACK_DATA_BANK_INCLUDED
#define YACK_DATA_BANK_INCLUDED 1

#include "yack/data/pool/cxx.hpp"


namespace yack
{


    //__________________________________________________________________________
    //
    //
    //! pool of C++ nodes
    //
    //__________________________________________________________________________
    template <typename NODE>
    class cxx_bank_of : public releasable
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef  cxx_pool_of<NODE> pool_type;  //!< alias
        typedef  NODE              node_type;  //!< alias
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit cxx_bank_of() throw() {} //!< create empty
        inline virtual ~cxx_bank_of() throw() {} //!< cleanup

        //______________________________________________________________________
        //
        // access methods
        //______________________________________________________________________
        inline const pool_type * operator->() const throw() { return &ready; } //!< access
        inline const pool_type & operator*()  const throw() { return  ready; } //!< access

        //______________________________________________________________________
        //
        // management methods
        //______________________________________________________________________
        inline void         deposit(NODE *node) throw() { assert(node); ready.store(node); } //!< node to bank
        inline virtual void release()           throw() { ready.release(); }                 //!< release content

        //! request a new node, create with fallback if empty
        template <typename T>
        inline NODE *request(const T &fallback) {
            return ready.size ? ready.query() : new NODE(fallback);
        }

        //! reserve with default value
        template <typename T>
        inline void reserve(size_t n, const T &args)
        {
            while (n-- > 0)
                ready.store( new NODE(args) );
        }



    private:
        YACK_DISABLE_COPY_AND_ASSIGN(cxx_bank_of);
        pool_type ready;
    };

}

#endif

