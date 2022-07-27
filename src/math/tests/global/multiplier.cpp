#include "yack/ordered/list.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace yack
{
    namespace math
    {
        template <typename T>
        class tagged_real
        {
        public:
            const T   value;
            const int exponent;
            
            inline   tagged_real(const T args) throw() :
            value(args),
            exponent(get_exp(value))
            {
            }
            
            inline ~tagged_real() throw() {}
            inline  tagged_real(const tagged_real &t) throw() :
            value(t.value),
            exponent(t.exponent)
            {
            }
            
            inline friend bool operator<(const tagged_real &lhs,
                                         const tagged_real &rhs) throw()
            {
                return lhs.exponent < rhs.exponent;
            }
            
            inline friend std::ostream & operator<<(std::ostream &os, const tagged_real &t)
            {
                os << t.value << "@" << t.exponent;
                return os;
            }
            
        private:
            YACK_DISABLE_ASSIGN(tagged_real);
            
            static inline int get_exp(const T x) throw()
            {
                int ex = 0;
                (void) std::frexp(x,&ex);
                return ex;
            }
        };
        
        template <typename T>
        class multiplier : public ordered_list< tagged_real<T> >
        {
        public:
            typedef ordered_list< tagged_real<T> >  self_type;
            typedef typename self_type::const_type  data_type;
            using self_type::insert;
            
            inline virtual ~multiplier() throw() {}
            inline explicit multiplier() throw() : self_type() {}
            
            inline void push(const T x)
            {
                data_type args(x);
                insert(args);
            }
            
            inline multiplier & operator<<(const T x)
            {
                push(x);
                return *this;
            }
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(multiplier);
            
        };
        
    }
}

YACK_UTEST(multiplier)
{

    {
        double x = 1.012;
        int    ex = 0;
        double sx = frexp(x,&ex);
        std::cerr << x << " -> " << sx << "*2^" << ex << std::endl;
    }

    {
        float x = -9e-16;
        int   ex = 0;
        float sx = frexpf(x,&ex);
        std::cerr << x << " -> " << sx << "*2^(" << ex << ")" << std::endl;
    }


    {
        long double x  = 3.14L;
        int         ex = 0;
        long double sx = frexpl(x,&ex);
        int         exx = 0;
        long double sxx = std::frexp(x,&exx);
        std::cerr << x << " -> " << sx << "*2^" << ex << std::endl;
        std::cerr << x << " -> " << sxx << "*2^" << exx << std::endl;

    }

    {
        math::multiplier<float> mm;
        mm << 1e-16 << 1e27 << 4.0f;
        std::cerr << "mm=" << mm  << std::endl;
    }

}
YACK_UDONE()

