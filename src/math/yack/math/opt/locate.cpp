


#include "yack/math/opt/locate.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/sort/network/sort4.hpp"
#include "yack/sort/network/sort5.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/signs.hpp"
#include "yack/sort/indexing.hpp"
#include "yack/math/numeric.hpp"
#include "yack/cameo/static-add.hpp"

#include <iomanip>

namespace yack
{
    namespace math
    {
        bool locate::verbose = false;
        static const char locate_inside[] = "// [locate.inside] ";
        static const char locate_expand[] = "// [locate.expand] ";

#define YACK_LOCATE(MSG) do { if(math::locate::verbose) { std::cerr << MSG << std::endl; } } while(false)


        namespace
        {
            template <typename T>
            struct Cubic
            {
                T f0, alpha, beta, gamma;

                inline T operator()(const T u) const noexcept
                {
                    return f0+u*(alpha+u*(beta+u*gamma));
                }
            };


            template <typename T>
            struct Quadratic
            {
                T A,B,C;

                inline T operator()(const T u) const noexcept
                {
                    return  u*(A*u+B)+C;
                }

                inline T zsearch(const T q0, const T q1) const noexcept
                {
                    static const T utol = numeric<T>::sqrt_eps;
                    assert(q0<0);
                    assert(q1>0);

                    // initialize
                    triplet<T>       u = {0,  -1, 1};
                    triplet<T>       f = {q0, -1, q1 };
                    if(update(u,f)) return u.b; // early return;
                    T u_old = u.b;

                    // loop
                ZSEARCH:
                    if(update(u,f)) return u.b; // early return;
                    const T err = std::abs(u.b-u_old);
                    if( err <= utol ) return u.b;
                    u_old = u.b;
                    goto ZSEARCH;
                }

            private:
                inline bool update(triplet<T> &u,
                                   triplet<T> &f) const noexcept
                {
                    static const T   h(0.5);
                    const Quadratic &F = *this;
                    f.b  = F( u.b = clamp(u.a,h*(u.a+u.c),u.c) );
                    switch( __sign::of(f.b) )
                    {
                        case __zero__:
                            return true;

                        case negative:
                            u.a = u.b;
                            f.a = f.b;
                            break;

                        case positive:
                            u.c = u.b;
                            f.c = f.b;
                            break;
                    }
                    return false;
                }

            };
            

        }
    }
}

#define real_t float
#define REAL(X) X##f
#include "locate.hxx"

#undef  real_t
#undef  REAL

#define real_t double
#define REAL(X) X
#include "locate.hxx"


#undef  real_t
#undef  REAL

#define real_t long double
#define REAL(X) X##L

#include "locate.hxx"


