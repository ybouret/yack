#include "yack/ipso/mul.hpp"
#include "yack/utest/run.hpp"
#include "yack/math/extended.hpp"
#include "yack/math/api.hpp"
#include "yack/ordered/heap.hpp"
#include "yack/ordered/run-time-buffer.hpp"
#include "yack/ordered/compiled-buffer.hpp"
#include "yack/apex.hpp"
#include "yack/ordered/roster.hpp"
#include "yack/system/rtti.hpp"
#include "yack/string.hpp"

using namespace yack;

namespace yack
{
    namespace ipso
    {

        //! default for apex types
        template <typename T>
        struct inside
        {
            typedef T type;
            static inline const type & send(const T    &args) noexcept { return args; }
            static inline const T    & recv(const type &args) noexcept { return args; }
        };

        template <>
        struct inside<float>
        {
            typedef extended<float> type;
            static inline type  send(const float args)   { return  args; }
            static inline float recv(const type &args)   { return *args; }

        };

        template <>
        struct inside<double>
        {
            typedef extended<double> type;
            static inline type   send(const double args) { return  args; }
            static inline double recv(const type  &args) { return *args; }
        };


        template <>
        struct inside<long double>
        {
            typedef extended<long double> type;
            static inline type        send(const long double args) { return  args; }
            static inline long double recv(const type       &args) { return *args; }
        };


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
                using  heap_type::size;
                using  heap_type::pull;
                using  heap_type::grow;

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                inline explicit proto() noexcept      : object_type(), heap_type()  {} //!< setup default
                inline explicit proto(const size_t n) : object_type(), heap_type(n) {} //!< setup with possible capacity
                inline virtual ~proto() noexcept                                    {} //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! convert and push value
                proto & operator << (param_type args) {
                    push( inside<type>::send(args) );
                    return *this;
                }

                //! get the current sum
                inline inside_type sum() {
                    switch(size())
                    {
                        case 0:  return 0;
                        case 1:  return pull();
                        default: break;
                    }
                    assert(size() > 1);
                REDUCE:
                    const inside_type lhs = pull(); assert(size() >= 1);
                    const inside_type rhs = pull();
                    const inside_type tmp = lhs + rhs;
                    if (size() <= 0) return tmp;
                    grow(tmp);
                    goto REDUCE;
                }

                void display_info() const {
                    static const string & myID = rtti::name<type>();
                    static const string & inID = rtti::name<inside_type>();
                    std::cerr << "add::proto<" << myID << "> working with <" << inID << ">" << std::endl;
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(proto);
            };
        };

        template <typename T>
        class add : public adding::proto<T, typename run_time_memory<T>::type >
        {
        public:
            YACK_DECL_ARGS(T,type);
            typedef typename inside<T>::type                             inside_type;
            typedef adding::proto<T, typename run_time_memory<T>::type > proto_class;

            inline explicit add() noexcept      : proto_class() {}
            inline explicit add(const size_t n) : proto_class(n) {}
            inline virtual ~add() noexcept {}

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

    std::cerr << dfa.sum() << std::endl;

    ipso::static_add<double,8> sda;
    sda << -0.1 << 3 << 0.2;
    std::cerr << "sda=" << sda << std::endl;
    std::cerr << sda.sum() << std::endl;

    ipso::add<apq> dqa(16);
    dqa << apq(-1,10) << apq(3) << apq(2,10);
    std::cerr << "dqa=" << dqa << std::endl;
    std::cerr << dqa.sum() << std::endl;


    dfa.display_info();
    sda.display_info();
    dqa.display_info();




}
YACK_UDONE()

