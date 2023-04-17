#include "yack/cameo/mul.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sequence/list.hpp"
#include "yack/ordered/roster.hpp"

using namespace yack;

namespace yack
{
    namespace cameo
    {

        template <typename> class  marked_real;
        template <typename> struct marked_data;

        template <>
        struct marked_data<float>
        {
            typedef int                info;
            typedef marked_real<float> real;

            sign_type   operator()(const real &, const real &);
            static info from(const float x) { int res=0;  (void) frexp(x,&res); return res; }
        };

        template <>
        struct marked_data<double>
        {
            typedef int                 info;
            typedef marked_real<double> real;

            static info from(const double x) { int res=0;  (void) frexp(x,&res); return res; }
            sign_type   operator()(const real &, const real &);
        };

        template <>
        struct marked_data<long double>
        {
            typedef int                 info;
            typedef marked_real<long double> real;

            static info from(const long double x) { int res=0;  (void) frexp(x,&res); return res; }
            sign_type   operator()(const real &, const real &);
        };

        



        template <typename T>
        class marked_real
        {
        public:
            YACK_DECL_ARGS(T,type);
            typedef marked_data<type>        data_type;
            typedef typename data_type::info info_type;

            inline explicit marked_real(param_type x) :
            value(x),
            field( data_type::from(value) )
            {
            }

            inline marked_real(const marked_real &other) noexcept :
            value(other.value),
            field(other.field)
            {

            }

            inline const_type & operator*() const noexcept { return value; }

            inline friend std::ostream & operator<<(std::ostream &os, const marked_real &self)
            {
                os << self.value << "[" << self.field << "]";
                return os;
            }

            const_type       value;
            const info_type  field;

        private:
            YACK_DISABLE_ASSIGN(marked_real);
        };

        sign_type marked_data<float>:: operator()(const real &lhs, const real &rhs)
        {
            return __sign::of(lhs.field,rhs.field);
        }

        sign_type marked_data<double>:: operator()(const real &lhs, const real &rhs)
        {
            return __sign::of(lhs.field,rhs.field);
        }

        sign_type marked_data<long double>:: operator()(const real &lhs, const real &rhs)
        {
            return __sign::of(lhs.field,rhs.field);
        }

        template <typename T>
        class marked_list : public roster< marked_real<T> , marked_data<T> >
        {
        public:
            YACK_DECL_ARGS(T,type);
            typedef marked_real<T>                real_type;
            typedef marked_data<T>                data_type;
            typedef roster<real_type,data_type>   self_type;
            typedef typename self_type::base_list base_type;

            using self_type::size;
            using self_type::lower;
            using self_type::upper;
            using self_type::insert;


            inline explicit marked_list() noexcept : self_type()
            {
            }

            inline virtual ~marked_list() noexcept
            {
            }

            inline void push(param_type args)
            {
                const real_type r(args);
                insert(r);
            }

            mutable_type prod()
            {

            LOOP:
                switch( size() )
                {
                    case 0: return 0;
                    case 1: return *lower();
                    case 2: return *lower() * *upper();
                    default:
                        assert( size()>2 );
                        const_type      l = *lower();
                        const_type      r = *upper();
                        const_type      p = l * r;
                        const real_type x(p);
                        insert(x);
                        goto LOOP;
                }
            }


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(marked_list);
        };

        template <typename T>
        static inline
        T generate(randomized::bits &ran)
        {
            static const T ten(10);
            static const T amp(20);
            const T rxp = ran.symm<T>();
            const T tmp = (ran.choice() ? -1 : 1) * std::pow(ten, amp*rxp);
            return tmp;
        }


        template <typename T>
        static inline void test_marked(randomized::bits &ran)
        {
            marked_list<T> Q;
            Q.push( generate<T>(ran) );
            Q.push( generate<T>(ran) );
            Q.push( generate<T>(ran) );
            std::cerr << "Q=" << Q << std::endl;
            std::cerr << "prod=" << Q.prod() << std::endl;
        }
    }
}

namespace {

	template <typename T>
	static inline void perform(randomized::bits& ran)
	{
		cameo::mul<T>  xmul;
		static const T ten(10);
		static const T amp(20);

		for (size_t n = 0; n <= 10; ++n)
		{
			xmul.free();
			T  res = n > 0 ? 1 : 0;
			for(size_t i=0;i<n;++i)
			{ 
				const T rxp = ran.symm<T>(); 
				const T tmp = (ran.choice() ? -1 : 1) * std::pow(ten, amp*rxp);
				res *= tmp;
				xmul << tmp;
			}
			std::cerr << res << " / " << xmul.product() << std::endl;
		}
	}

    template <typename T>
    static inline void perform_cplx(randomized::bits& ran)
    {
        typedef complex<T> cplx_t;
        cameo::mul<cplx_t> xmul;
        static const T ten(10);
        static const T amp(20);

        for (size_t n = 0; n <= 10; ++n)
        {
            xmul.free();
            cplx_t  res = n > 0 ? 1 : 0;
            for(size_t i=0;i<n;++i)
            {
                const T re = (ran.choice() ? -1 : 1) * std::pow(ten, amp*ran.symm<T>() );
                const T im = (ran.choice() ? -1 : 1) * std::pow(ten, amp*ran.symm<T>() );
                const cplx_t tmp(re,im);
                res *= tmp;
                xmul << tmp;
            }
            std::cerr << res << " / " << xmul.product() << std::endl;
        }
    }

}

YACK_UTEST(cameo_mul)
{
	randomized::rand_ ran;

    cameo::test_marked<float>(ran);
    cameo::test_marked<double>(ran);
    cameo::test_marked<long double>(ran);

    return 0;

	{ cameo::mul<float>  xmul; }
	{ cameo::mul<double> xmul(7); }

    std::cerr << "-- reals" << std::endl;
	perform<float>(ran);
	perform<double>(ran);
	perform<long double>(ran);

    std::cerr << "-- complexes" << std::endl;
    perform_cplx<float>(ran);
    perform_cplx<double>(ran);
    perform_cplx<long double>(ran);

    std::cerr << "-- special cases" << std::endl;
	{
		cameo::mul<double> xm;
		xm.push(3, 3);
		std::cerr << xm << std::endl;
		xm.ipower(4, 2);
		xm.ipower(4, -3);
		std::cerr << xm << std::endl;
	}
}
YACK_UDONE()
