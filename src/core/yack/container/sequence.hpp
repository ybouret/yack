
//! \file

#ifndef YACK_SEQUENCE_INCLUDED
#define YACK_SEQUENCE_INCLUDED 1

#include "yack/container.hpp"
#include "yack/type/args.hpp"
#include "yack/signs.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! sequence of objects
    //
    //__________________________________________________________________________
    template <typename T>
    class sequence : public container
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type); //!< aliases

        //______________________________________________________________________
        //
        // virtual interface
        //______________________________________________________________________
        virtual void push_back(param_type args)  = 0; //!< push back existing object
        virtual void push_front(param_type args) = 0; //!< push front existing object
        virtual void pop_back()  noexcept = 0;         //!< pop back object
        virtual void pop_front() noexcept = 0;         //!< pop front object

        //______________________________________________________________________
        //
        // non-virtual interface
        //______________________________________________________________________
        inline type       &front()       noexcept { return *(type *) _front(); } //!< front item
        inline const_type &front() const noexcept { return *         _front(); } //!< front item, const
     
        inline type       &back()       noexcept { return *(type *) _back(); }   //!< back item
        inline const_type &back() const noexcept { return *         _back(); }   //!< back item, const

        //! adjust size
        inline void adjust(const size_t n, param_type pad)
        {
            switch( __sign::of(n,size()) )
            {
                case negative:
                    assert(n<size());
                    while(n<size())
                        pop_back();
                    break;

                case __zero__: assert(size()==n); break;
                case positive:
                    assert(size()<n);
                    while(size()<n)
                        push_back(pad);
                    break;
            }
        }

        //! make size
        inline void make(const size_t n, param_type pad)
        {
            free();
            while(size()<n) push_back(pad);
        }

        //! helper
        inline sequence & operator<<(param_type args)
        {
            push_back(args);
            return *this;
        }

        //! return copy of removed back value
        inline type pull_back() { const_type tmp = back(); pop_back(); return tmp; }

        //! return copy of removed front value
        inline type pull_front() { const_type tmp = front(); pop_front(); return tmp; }


        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~sequence() noexcept {}
    protected:
        inline explicit sequence() noexcept : container() {}
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(sequence);
        virtual const_type *_front() const noexcept = 0;
        virtual const_type *_back()  const noexcept = 0;

    };
    
}


#endif

