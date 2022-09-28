


#include "yack/math/opt/locate.hpp"
#include "yack/sequence/thin-array.hpp"
#include "yack/sort/network/sort4.hpp"
#include "yack/sort/network/sort5.hpp"
#include "yack/ios/ocstream.hpp"
#include "yack/signs.hpp"
#include "yack/sort/indexing.hpp"

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

                inline T operator()(const T u) const throw()
                {
                    return f0+u*(alpha+u*(beta+u*gamma));
                }
            };


            template <typename T>
            struct Quadratic
            {
                T A,B,C;

                inline T operator()(const T u) const throw()
                {
                    return  u*(A*u+B)+C;
                }

                inline T slope(const T u) const throw()
                {
                    return twice(A*u)+B;
                }
            };

            template <typename T>
            struct Affine
            {
                T start;
                T slope;

                inline T operator()(const T u) const throw()
                {
                    return start + u * slope;
                }
            };
            

        }
    }
}

#define real_t float
#include "locate.hxx"

#undef  real_t
#define real_t double
#include "locate.hxx"


#undef  real_t
#define real_t long double
#include "locate.hxx"


