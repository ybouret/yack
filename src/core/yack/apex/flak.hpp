#ifndef YACK_APEX_FLAK_INCLUDED
#define YACK_APEX_FLAK_INCLUDED 1

#include "yack/apex.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/math/api.hpp"

namespace yack
{
    namespace apex
    {

        namespace internal
        {
            template <typename U, typename V>
            void add_prod_to(apq &p, const U &u, const V &v)
            {
                const apq u_(u);
                const apq v_(v);
                p += u_ * v_;
            }

            template <typename U>
            void add_prod_to(apq &p, const U &u, const apq &v)
            {
                const apq u_(u);
                p += u_ * v;
            }

            template <typename V>
            void add_prod_to(apq &p, const apq &u, const V &v)
            {
                const apq v_(v);
                p += u * v_;
            }

            template <typename T>
            void add_mod2_to(apq &p2, const T &t)
            {
                const apq t_(t);
                p2 += t_.mod2();
            }

            template <>
            void add_mod2_to(apq &p2, const apq &t)
            {
                p2 += t.mod2();
            }

        }


        struct flak
        {
            typedef cxx_array<apq,memory::dyadic> qarray;

            //__________________________________________________________________
            //
            //! find common denominator of an array of rationals
            //__________________________________________________________________
            template <typename ARRAY> static inline
            apn common_denominator(ARRAY &arr)
            {
                const size_t n = arr.size();
                switch(n)
                {
                    case 0: return 1;
                    case 1: return arr[1].den;
                    default:
                        break;
                }
                assert(n>=2);
                apn res = apn::lcm_(arr[1].den,arr[2].den);
                for(size_t i=3;i<=n;++i)
                    res = apn::lcm_(res,arr[i].den);
                return res;
            }



            template <typename ALPHA, typename BETA> static inline
            bool colinear(ALPHA &alpha, BETA &beta) {
                assert(alpha.size()==beta.size());
                const size_t n = alpha.size();
                apq          a2; // |alpha|^2
                apq          ab; //  alpha * beta
                apq          b2; // |beta|^2

                for(size_t i=n;i>0;--i)
                {
                    typename ALPHA::const_type &alpha_i = alpha[i];
                    typename BETA::const_type  &beta_i  = beta[i];
                    internal::add_prod_to(ab, alpha_i, beta_i);
                    internal::add_mod2_to(b2, beta_i);
                    internal::add_mod2_to(a2, alpha_i);
                }

                if(a2.num.s==__zero__ && b2.num.s==__zero__)
                {
                    //                  -->
                    // both vectors are nul
                    return true;
                }
                else
                {
                    assert(a2>0 || b2>0);
                    if(ab.num.s==__zero__)
                    {
                        //               -->
                        // one vector is nul or orthogonality
                        return false;
                    }
                    else
                    {
                        //                     -->
                        // both vector are NOT nul
                        assert(ab!=0);
                        assert(a2>0);
                        assert(b2>0);
                        for(size_t i=n;i>0;--i)
                        {
                            const apq lhs = b2 * alpha[i];
                            const apq rhs = ab * beta[i];
                            if(lhs!=rhs) return false;
                        }
                        return true;
                    }
                }
            }


        };

    }
}

#endif

