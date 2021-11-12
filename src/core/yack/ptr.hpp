//! \file

#ifndef YACK_PTR_INCLUDED
#define YACK_PTR_INCLUDED 1

#include "yack/type/args.hpp"

namespace yack
{
 
    class ptr_
    {
    public:
        static const char nil[];
        virtual ~ptr_() throw();
        
    protected:
        explicit ptr_() throw();
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(ptr_);
    };
    
    template <typename T>
    class ptr : public ptr_
    {
    public:
        YACK_DECL_ARGS(T,type);
        inline virtual ~ptr() throw() { assert(NULL==pointee); }
        
        
        inline type       & operator*()       throw() { assert(pointee); return *pointee; }
        inline const_type & operator*() const throw() { assert(pointee); return *pointee; }
        
        inline type       * operator->()       throw() { return pointee; }
        inline const_type * operator->() const throw() { return pointee; }
        
        template <typename OSTREAM> inline
        friend OSTREAM & operator<<(OSTREAM &os, const ptr<T> &self)
        {
            if(self.pointee) { os << *self.pointee; } else { os << nil; }
            return os;
        }
        
        
    protected:
        explicit ptr(type *addr) throw() :
        pointee( (mutable_type *) addr )
        {
        }
        
        mutable_type *pointee;
        
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(ptr);
    };
    
    
}
#endif

