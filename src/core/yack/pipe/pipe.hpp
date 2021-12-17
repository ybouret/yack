//! \file

#ifndef YACK_PIPE_INCLUDED
#define YACK_PIPE_INCLUDED 1

#include "yack/type/args.hpp"
#include "yack/object.hpp"
#include "yack/container/as-capacity.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! base class for FIFO/LIFO pipe
    //
    //__________________________________________________________________________
    class pipe
    {
    public:
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________

        //! I/O policy
        enum policy
        {
            lifo, //!< Last In, First Out
            fifo  //!< First In, First Out
        };

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const policy scheme; //!< the policy for this pipe

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~pipe()    throw(); //!< cleanup

    protected:
        pipe(const policy) throw(); //!< setup policy
        pipe(const pipe &) throw(); //!< copy policy

    private:
        YACK_DISABLE_ASSIGN(pipe);
    };

    //__________________________________________________________________________
    //
    //
    //! interface for generic pipe
    //
    //__________________________________________________________________________
    template <typename T, const pipe::policy SCHEME>
    class pipe_of : public pipe
    {
    public:
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);                    //!< aliases
        static const policy class_scheme = SCHEME; //!< alias

        //______________________________________________________________________
        //
        // interface
        //______________________________________________________________________
        virtual void        push(param_type)     = 0; //!< push an object
        virtual void        pop()  throw()       = 0; //!< remove object according to policy
        virtual type       &peek() throw()       = 0; //!< next available object
        virtual const_type &peek() const throw() = 0; //!< next available const object

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~pipe_of()        throw() {} //!< cleanup

    protected:
        inline explicit pipe_of()        throw() : pipe(class_scheme) {} //!< setup with scheme
        inline pipe_of(const pipe_of &_) throw() : pipe(_)            {} //!< copy scheme

    private:
        YACK_DISABLE_ASSIGN(pipe_of);
    };

    //__________________________________________________________________________
    //
    //
    //! build pipes from sequences
    //
    //__________________________________________________________________________
    template <typename T, const pipe::policy SCHEME, typename SEQUENCE>
    class pipe_on : public pipe_of<T,SCHEME>, public SEQUENCE
    {
    public:
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);               //!< aliases
        typedef pipe_of<T,SCHEME> pipe_type;  //!< alias
        typedef SEQUENCE          self_type;  //!< alias

        //______________________________________________________________________
        //
        // interface
        //______________________________________________________________________

        //! always push back
        inline virtual void push(param_type args)
        {
            this->push_back(args);
        }

        //! pop: lifo=back, fifo=front
        inline virtual void pop() throw()
        {
            static const int2type<SCHEME> manner = {};
            pop_(manner);
        }

        //! peek: lifo=back, fifo=front
        inline virtual const_type &peek() const throw()
        {
            static const int2type<SCHEME> manner = {};
            return peek_(manner);
        }

        //! alias
        inline virtual type &peek()   throw()
        {
            static const int2type<SCHEME> manner = {};
            return coerce(peek_(manner));
        }

        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~pipe_on() throw() {} //!< cleanup

    protected:
        inline explicit pipe_on()        throw() : pipe_type(),  self_type()    {} //!< default setup
        inline pipe_on(const pipe_on &_) throw() : pipe_type(_), self_type(_)   {} //!< copy using sequence
        inline pipe_on(const size_t n) : pipe_type(),  self_type(n,as_capacity) {} //!< setup with capacity

    private:
        YACK_DISABLE_ASSIGN(pipe_on);
        inline void pop_(const int2type<pipe::lifo>&) throw() { this->pop_back();  }
        inline void pop_(const int2type<pipe::fifo>&) throw() { this->pop_front(); }

        inline const_type &peek_(const int2type<pipe::lifo>&) const throw() { return this->back();  }
        inline const_type &peek_(const int2type<pipe::fifo>&) const throw() { return this->front(); }


    };


}

#endif

