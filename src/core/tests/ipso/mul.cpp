#include "yack/ipso/mul.hpp"
#include "yack/utest/run.hpp"
#include "yack/math/extended.hpp"
#include "yack/math/api.hpp"
#include "yack/ordered/heap.hpp"
#include "yack/ordered/run-time-buffer.hpp"
#include "yack/ordered/compiled-buffer.hpp"
#include "yack/apex.hpp"

using namespace yack;

namespace yack
{
    namespace ipso
    {

        template <typename T>
        struct inside
        {
            typedef T type;
            static inline  const type & conv(const T &args) noexcept { return args; }
        };

        template <>
        struct inside<float>
        {
            typedef extended<float> type;
            static inline  type conv(const float f) { return f; }
        };

#if 1
        template <>
        struct inside<double>
        {
            typedef extended<double> type;
            static inline  type conv(const double f) { return f; }
        };
#endif

        template <typename T>
        struct run_time_memory
        {
            typedef run_time_buffer< typename inside<T>::type, memory_type> type;
        };

        template <typename T, const size_t N>
        struct compiled_memory
        {
            typedef compiled_buffer< typename inside<T>::type, N> type;
        };



        struct adding
        {

            //__________________________________________________________________
            //
            //! comparator of inside<T>
            //__________________________________________________________________
            template <typename T> struct  comparator
            {
                typedef typename inside<T>::type               inside_type;
                typedef typename scalar_for<inside_type>::type scalar_type;


                inline int operator()(const inside_type & lhs, const inside_type & rhs) const
                {
                    const scalar_type L = math::abs_of(lhs);
                    const scalar_type R = math::abs_of(rhs);
                    return (L < R) ? 1 : ((R < L) ? -1 : 0);
                }
            };

            template <typename T, typename MEM_BUFFER>
            class proto : public object_type, public heap<typename inside<T>::type,comparator<T>,MEM_BUFFER>
            {
            public:
                YACK_DECL_ARGS(T,type);
                typedef typename inside<type>::type              inside_type;
                typedef comparator<type>                         comparator_;
                typedef heap<inside_type,comparator_,MEM_BUFFER> heap_type;

                using  heap_type::push;

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                inline explicit proto() noexcept      : object_type(), heap_type()  {} //!< setup default
                inline explicit proto(const size_t n) : object_type(), heap_type(n) {} //!< setup with possible capacity
                inline virtual ~proto() noexcept                                    {} //!< cleanup

                proto & operator << (param_type args) {
                    push( inside<type>::conv(args) );
                    return *this;
                }

            };
        };

        template <typename T>
        class add : public adding::proto<T, typename run_time_memory<T>::type >
        {
        public:
            YACK_DECL_ARGS(T,type);
            typedef typename inside<T>::type                             inside_type;
            typedef adding::proto<T, typename run_time_memory<T>::type > proto_class;

            inline explicit add() noexcept : proto_class() {}
            inline virtual ~add() noexcept {}
            inline explicit add(const size_t n) : proto_class(n) {}

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(add);
        };

        template <typename T, const size_t N>
        class static_add : public adding::proto<T, typename compiled_memory<T,N>::type >
        {
        public:
            YACK_DECL_ARGS(T,type);
            typedef typename inside<T>::type                               inside_type;
            typedef adding::proto<T, typename compiled_memory<T,N>::type > proto_class;

            inline explicit static_add() noexcept : proto_class() {}
            inline virtual ~static_add() noexcept {}

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(static_add);
        };


    }

}

YACK_UTEST(ipso_mul)
{

    ipso::add<float> dfa;
    dfa << -0.1 << 3 << 0.2;
    std::cerr << "dsa=" << dfa << std::endl;

    ipso::static_add<double,8> sda;
    sda << -0.1 << 3 << 0.2;
    std::cerr << "sda=" << sda << std::endl;




}
YACK_UDONE()

