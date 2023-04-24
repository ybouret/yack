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
            static inline sign_type    comp(const type &l, const type &r) {
                return l<r ? negative : (r<l ? positive : __zero__);
            }
        };

        //! extended reals ops
        template <typename XREAL>
        struct xr_ops
        {
            static inline sign_type compare(const XREAL &l,
                                            const XREAL &r) noexcept
            {
                return __sign::of(l.p,r.p);
            }
        };

        template <>
        struct inside<float>
        {
            typedef extended<float> type;
            static inline type      send(const float args)   { return  args; }
            static inline float     recv(const type &args)   { return *args; }
            static inline sign_type comp(const type &l, const type &r)
            {
                return xr_ops<type>::compare(l,r);
            }

        };

        template <>
        struct inside<double>
        {
            typedef extended<double> type;
            static inline type       send(const double args) { return  args; }
            static inline double     recv(const type  &args) { return *args; }
            static inline sign_type  comp(const type &l, const type &r)
            {
                return xr_ops<type>::compare(l,r);
            }
        };


        template <>
        struct inside<long double>
        {
            typedef extended<long double> type;
            static inline type            send(const long double args) { return  args; }
            static inline long double     recv(const type       &args) { return *args; }
            static inline sign_type       comp(const type &l, const type &r)
            {
                return xr_ops<type>::compare(l,r);
            }
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


        template <typename T>
        class api : public object_type
        {
        public:
            YACK_DECL_ARGS(T,type);
            typedef typename inside<T>::type inside_type;
            inline virtual ~api() noexcept {}

            // virtual interface
            virtual void        append(const inside_type &) = 0; //!< append an inside type
            virtual inside_type reduce() = 0;                    //!< reduce to inside type

            // non-virtual interface

            //! high-level convert and push value
            inline api & operator<<(param_type args) {
                append( inside<type>::send(args) );
                return *this;
            }

            //! high-level reduction and conversiont
            inline mutable_type operator*() {
                const inside_type res = reduce();
                return inside<T>::recv(res);
            }


        protected:
            inline explicit api(container &_) noexcept : object_type(), super(_) {}

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(api);
            container &super;
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
            class proto : public heap<typename inside<T>::type,comparator<T>,MEM_BUFFER>,  public api<T>
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
                inline explicit proto() noexcept      : heap_type(),  api<T>(static_cast<container&>(*this)) {} //!< setup default
                inline explicit proto(const size_t n) : heap_type(n), api<T>(static_cast<container&>(*this)) {} //!< setup with possible capacity
                inline virtual ~proto() noexcept                                                             {} //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                inline virtual void append(const inside_type &args) {
                    push(args);
                }


                //! get the current sum
                inline virtual inside_type reduce() {
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


        template <typename T> struct inside_comparator
        {
            typedef typename inside<T>::type inside_type;
            inline sign_type operator()(const inside_type &l,
                                        const inside_type &r) noexcept
            {
                return inside<T>::comp(l,r);
            }
        };

        template <typename T>
        class mul : public roster< typename inside<T>::type, inside_comparator<T> >, public api<T>
        {
        public:
            YACK_DECL_ARGS(T,type);
            typedef typename inside<T>::type        inside_type;
            typedef inside_comparator<type>         incomp_type;
            typedef roster<inside_type,incomp_type> proto_class;
            using proto_class::insert;
            using proto_class::size;
            using proto_class::lower;
            using proto_class::upper;

            inline explicit mul() noexcept : proto_class(), api<T>( static_cast<container&>(*this) ) {}
            inline virtual ~mul() noexcept {}


            inline virtual void append(const inside_type &args)
            {
                insert(args);
            }


            inline virtual inside_type reduce()
            {
                switch( size() )
                {
                    case 0: { const inside_type res(0); return res; }
                    case 1: return lower();
                    default:
                        break;
                }

                assert(size()>=2);

            REDUCE:
                const inside_type lhs = lower();
                const inside_type rhs = upper();
                const inside_type res = lhs*rhs;
                if(size()<=0) return res;
                insert(res);
                goto REDUCE;
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(mul);
        };



    }

}

YACK_UTEST(ipso_mul)
{

    ipso::add<float> dfa;
    dfa << -0.1 << 3 << 0.2;
    std::cerr << "dsa=" << dfa << std::endl;

    std::cerr << dfa.reduce() << std::endl;

    ipso::static_add<double,8> sda;
    sda << -0.1 << 3 << 0.2;
    std::cerr << "sda=" << sda << std::endl;
    std::cerr << sda.reduce() << std::endl;

    ipso::add<apq> dqa(16);
    dqa << apq(-1,10) << apq(3) << apq(2,10);
    std::cerr << "dqa=" << dqa << std::endl;
    std::cerr << dqa.reduce() << std::endl;


    dfa.display_info();
    sda.display_info();
    dqa.display_info();


    ipso::mul<float> dfm;
    dfm << -0.01 << 30 << 0.2;
    std::cerr << "dfm=" << dfm << std::endl;
    std::cerr << dfm.reduce() << std::endl;

    ipso::mul<apq> dqm;
    dqm << apq(-1,100) << apq(30) << apq(2,10);
    std::cerr << "dqm=" << dqm << std::endl;
    std::cerr << dqm.reduce() << std::endl;


}
YACK_UDONE()

