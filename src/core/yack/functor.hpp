//! \file
#ifndef YACK_FUNCTOR_INCLUDED
#define YACK_FUNCTOR_INCLUDED 1


#include "yack/functor/callback.hpp"
#include "yack/functor/command.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //! functor
    /**
     - R     is the return type
     - TLIST is the parameters type list
     */
    //__________________________________________________________________________
    template <typename R, class TLIST>
    class functor : public kernel::callable<R,TLIST>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef R                                   result;    //!< alias for result
        typedef TLIST                               arguments; //!< alias for type list
        typedef typename kernel::callable<R,TLIST>  callable;  //!< base class
        YACK_FUNCTOR_PARAMETERS();                             //!< alias for parameters


        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! destroy the internal wrappers
        inline virtual ~functor() throw() { assert(function_); destroy(); }


        //! copy: duplicate the functor context
        inline functor( const functor &other ) :
        callable(), function_( other.function_->clone() )
        { assert( function_ ); }

        //! assign: duplicate source context
        inline functor & operator=( const functor &other )
        {
            if( this != &other )
            {
                //-- unsafe --
                callable *fn = other.function_->clone(); assert(fn);
                //--  safe  --
                destroy();
                function_ = fn;
            }
            return *this;
        }

        //! functionoid constructor
        /**
         \param functionoid a callable object
         functionoid can be of TWO types
         - it can be a regular C function, whose arguments and return types are compatible
         with the functor prototype.
         - it can be a full C++ object with an operator()(...)  whose arguments and return types are compatible
         with the functor prototype. In that case, THE C++ OBJECT IS COPIED, i.e. the whole
         context is wrapped in that functor.

         This is achieved through the kernel::callback which uses the copy semantic of FUNCTION.
         */
        template <typename FUNCTION>
        inline functor( const FUNCTION &functionoid ) : callable(),
        function_( new kernel::callback<R,TLIST,FUNCTION>(functionoid) ) {}

        //! object+method constructor
        /**
         \param o an object pointer
         \param m an object method, like &OBJECT_POINTER::DoSomething(...)
         Since only the pointer is copied, this strategy can be used
         for building one functor whose context can be changed by tweaking the
         original object.
         */
        template <typename OBJECT_POINTER, typename METHOD_POINTER>
        inline functor( OBJECT_POINTER o, METHOD_POINTER m ) :  callable(),
        function_( new kernel::command<R,TLIST,OBJECT_POINTER,METHOD_POINTER>( o, m ) ) {}

        //______________________________________________________________________
        //
        // clonable interface
        //______________________________________________________________________
        virtual callable  *clone() const { return new functor(*this); } //!< cloneable interface, using copy constructor


        //______________________________________________________________________
        //
        // forwarding calls, using optimized parameter types
        // to avoid copy overhead
        //______________________________________________________________________

        //! forward no argument call
        inline R operator()(void) {
            assert(function_);
            return (*function_)();
        }

        //! forward one argument call
        inline R operator()(param1 p1)
        {
            assert(function_);
            return (*function_)(p1);
        }

        //! forward two arguments call
        inline R operator()(param1 p1,
                            param2 p2)
        {
            assert( function_ );
            return (*function_)(p1,p2);
        }

        //! forward three arguments call
        inline R operator()(param1 p1,
                            param2 p2,
                            param3 p3)
        {
            assert( function_ );
            return (*function_)(p1,p2,p3);
        }

        //! forward four arguments call
        inline R operator()(param1 p1,
                            param2 p2,
                            param3 p3,
                            param4 p4)
        {
            assert( function_ );
            return (*function_)(p1,p2,p3,p4);
        }

        //! direct assignation for binder_first or manual setting
        explicit functor( callable *proc ) throw() : function_( proc ) { assert( proc ); }

    private:
        callable  *function_;
        inline void destroy() throw()
        {
            object *pObj = function_; // __DMC__ bug otherwise
            function_    = NULL;
            delete  pObj;
        }
    };


    namespace kernel {

        //! first parameter binding
        /**
         inner typedefs perform all the types computations
         */
        template <typename incoming>
        class binder_first : public callable<typename incoming::result, typename incoming::arguments::tail>
        {
        public:
            typedef typename incoming::result                     R;        //!< return type alias
            typedef typename incoming::arguments::tail            TLIST;    //!< arguments alias
            typedef functor<R,TLIST>                              outgoing; //!< return functor alias

            typedef typename incoming::arguments::head            bounded;       //!< bounded type
            typedef typename type_traits<bounded>::parameter_type bounded_param; //!< bounded parameter type

            //! cloneable interface by copy
            virtual callable<R,TLIST> *clone() const { return new binder_first( *this ); }

            //! constructor
            explicit binder_first( const incoming &f, bounded_param b ) :
            functor_( f ),
            bounded_( b )
            {}

            //! destructor
            virtual ~binder_first() throw() {}


            YACK_FUNCTOR_PARAMETERS(); //!< parameters alias

            //! 1->0 argument call
            inline R operator()(void)
            {
                return functor_(bounded_);
            }

            //! 2->1 argument call
            inline R operator()(param1 p1)
            {
                return functor_(bounded_,p1);
            }

            //! 3->2 arguments call
            inline R operator()(param1 p1, param2 p2)
            {
                return functor_(bounded_,p1,p2);
            }


        private:
            incoming functor_;   //!< functor copy semantics
            bounded  bounded_;   //!< first parameter value

            binder_first( const binder_first &other ) :
            functor_( other.functor_ ),
            bounded_( other.bounded_ )
            {
            }
        };
    }

    //! bind first parameter to make a new functor
    template <typename incoming>
    inline
    typename kernel::binder_first<incoming>::outgoing
    bind_first( const incoming &f, typename kernel::binder_first<incoming>::bounded_param p )
    {
        typedef typename kernel::binder_first<incoming>::outgoing out_functor;
        typedef typename out_functor::callable                    out_callable;
        out_callable *proc = new kernel::binder_first<incoming>( f, p );
        return  out_functor( proc );
    }

    //! make a functor from a C function
    template <typename R,typename T>
    inline functor<R,TL1(T)> cfunctor( R (*cfn)(T) ) { return functor<R,TL1(T)>( cfn ); }

    //! make a functor from a C function
    template <typename R,typename T,typename U>
    inline functor<R,TL2(T,U)> cfunctor2( R (*cfn)(T,U) ) { return functor<R,TL2(T,U)>( cfn ); }

    //! make a functor from a C function
    template <typename R,typename T,typename U,typename V>
    inline functor<R,TL3(T,U,V)> cfunctor3( R (*cfn)(T,U,V) ) { return functor<R,TL3(T,U,V)>( cfn ); }

}

#endif
