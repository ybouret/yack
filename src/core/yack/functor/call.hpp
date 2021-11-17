//! \file
#ifndef YACK_FUNCTOR_CALL_INCLUDED
#define YACK_FUNCTOR_CALL_INCLUDED 1


#include "yack/object.hpp"
#include "yack/type/traits.hpp"

namespace yack
{

    namespace kernel
    {

        //______________________________________________________________________
        //
        //
        //! callable interface construction
        //
        //______________________________________________________________________

#define YACK_CALLABLE_PROTOTYPE()                 \
/**/  private:                                    \
/**/   YACK_DISABLE_COPY_AND_ASSIGN(callable);    \
/**/  public:                                     \
/**/    explicit callable() throw() : object() {} \
/**/    virtual ~callable() throw() {}            \
/**/    virtual callable *clone() const = 0


        //! memory management
#define YACK_CALLABLE_OBJECT() : public  object
        //! defines the behavior of a generic callable object
        /**
         the parameters are send thru a typelist
         */
        template <typename R,class TLIST>
        class callable YACK_CALLABLE_OBJECT()
        {
            YACK_CALLABLE_PROTOTYPE();
        };

        //! no argument callable type
        template <typename R>
        class callable<R,null_type> YACK_CALLABLE_OBJECT()
        {
            YACK_CALLABLE_PROTOTYPE();
            virtual R operator() (void) = 0; //!< interface for no argument call
        };

        //! one argument callable type
        template <typename R, typename P1>
        class callable<R,TL1(P1)> YACK_CALLABLE_OBJECT()
        {
            YACK_CALLABLE_PROTOTYPE();
            virtual R operator()( typename type_traits<P1>::parameter_type ) = 0;
        };

        //! two arguments callable type
        template <typename R, typename P1, typename P2>
        class callable<R,TL2(P1,P2)> YACK_CALLABLE_OBJECT()
        {
            YACK_CALLABLE_PROTOTYPE();
            virtual R operator()(typename type_traits<P1>::parameter_type,
                                 typename type_traits<P2>::parameter_type)= 0;
        };

        //! three arguments callable type
        template <typename R, typename P1, typename P2, typename P3>
        class callable<R,TL3(P1,P2,P3)> YACK_CALLABLE_OBJECT()
        {
            YACK_CALLABLE_PROTOTYPE();
            virtual R operator()(typename type_traits<P1>::parameter_type,
                                 typename type_traits<P2>::parameter_type,
                                 typename type_traits<P3>::parameter_type )= 0;
        };

        //! four arguments callable type
        template <typename R, typename P1, typename P2, typename P3, typename P4>
        class callable<R,TL4(P1,P2,P3,P4)> YACK_CALLABLE_OBJECT()
        {
            YACK_CALLABLE_PROTOTYPE();
            virtual R operator()(typename type_traits<P1>::parameter_type,
                                 typename type_traits<P2>::parameter_type,
                                 typename type_traits<P3>::parameter_type,
                                 typename type_traits<P4>::parameter_type
                                 ) = 0;
        };

        //! compute parameters aliases
#define YACK_FUNCTOR_PARAMETERS() \
typedef typename type_traits< typename tl::safe_type_at<TLIST,0,empty_type>::result>::parameter_type param1;\
typedef typename type_traits< typename tl::safe_type_at<TLIST,1,empty_type>::result>::parameter_type param2;\
typedef typename type_traits< typename tl::safe_type_at<TLIST,2,empty_type>::result>::parameter_type param3;\
typedef typename type_traits< typename tl::safe_type_at<TLIST,3,empty_type>::result>::parameter_type param4


    }

}

#endif
