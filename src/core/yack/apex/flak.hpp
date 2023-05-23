#ifndef YACK_APEX_FLAK_INCLUDED
#define YACK_APEX_FLAK_INCLUDED 1

#include "yack/apex.hpp"
#include "yack/math/api.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{
    namespace apex
    {

        namespace internal
        {
            //! add product to a rational
            template <typename U,typename V> struct add_prod {
                static inline void to(apq &p, const U &u, const V &v)
                {
                    const apq u_(u);
                    const apq v_(v);
                    p += u_ * v_;
                } //!< implementation
            };

            //! add product to a rational
            template <typename U> struct add_prod<U,apq> {
                static inline void to(apq &p, const U &u, const apq &v)
                {
                    const apq u_(u);
                    p += u_ * v;
                } //!< implementation
            };

            //! add product to a rational
            template <typename V> struct add_prod<apq,V> {
                static inline void to(apq &p, const apq &u, const V &v)
                {
                    const apq v_(v);
                    p += u * v_;
                } //!< implementation
            };

            //! add product to a rational
            template <> struct add_prod<apq,apq> {
                static inline void to(apq &p, const apq &u, const apq &v)
                {
                    p += u * v;
                } //!< implementation
            };

            template <typename LHS, typename RHS>   inline
            apq dot(LHS &lhs, RHS &rhs)
            {
                assert(lhs.size()==rhs.size());
                apq res;
                for(size_t i=lhs.size();i>0;--i)
                    internal::add_prod<typename LHS::type,typename RHS::type>::to(res, lhs[i], rhs[i]);
                return res;
            }


            //! add mod2 to a rational
            template <typename T> struct add_mod2
            {
                static inline void to(apq &p2, const T &t)
                {
                    const apq t_(t);
                    p2 += t_.mod2();
                } //!< implementation
            };

            //! add mod2 to a rational
            template <>
            struct add_mod2<apq>
            {
                static inline void to(apq &p2, const apq &t)
                {
                    p2 += t.mod2();
                } //!< implementation
            };

            template <typename LHS>   inline
            apq mod2(LHS &lhs)
            {
                apq res;
                for(size_t i=lhs.size();i>0;--i)
                    internal::add_mod2<typename LHS::type>::to(res, lhs[i]);
                return res;
            }

        }

        //! Linear Algebra Kernel
        struct flak
        {

            //__________________________________________________________________
            //
            //! raise specific error
            //__________________________________________________________________
            static void raise_error(const char *msg);

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

            //__________________________________________________________________
            //
            //! convert rationals to integers representation
            //__________________________________________________________________
            static void simplify(writable<apq> &q);

            //__________________________________________________________________
            //
            //! simplify all rows
            //__________________________________________________________________
            static void simplify_rows(matrix<apq> &M);

            //__________________________________________________________________
            //
            //! convert rationals to integers with univocal sign
            //__________________________________________________________________
            static void univocal(writable<apq> &q);

            //__________________________________________________________________
            //
            //! make all univocal rows
            //__________________________________________________________________
            static void univocal_rows(matrix<apq> &M);



            //__________________________________________________________________
            //
            //
            //! generic colinearity detection
            //
            //__________________________________________________________________
            template <typename ALPHA, typename BETA> static inline
            bool colinear(ALPHA &alpha, BETA &beta) {
                assert(alpha.size()==beta.size());

                //--------------------------------------------------------------
                // initialize
                //--------------------------------------------------------------
                const size_t n = alpha.size();
                apq          a2; // |alpha|^2
                apq          ab; //  alpha * beta
                apq          b2; // |beta|^2

                //--------------------------------------------------------------
                // compute metrics
                //--------------------------------------------------------------
                for(size_t i=n;i>0;--i)
                {
                    typename ALPHA::const_type &alpha_i = alpha[i];
                    typename BETA::const_type  &beta_i  = beta[i];
                    internal::add_prod<typename ALPHA::type, typename BETA::type>::to(ab, alpha_i, beta_i);
                    internal::add_mod2<typename BETA::type> ::to(b2, beta_i);
                    internal::add_mod2<typename ALPHA::type>::to(a2, alpha_i);
                }

                if(a2.num.s==__zero__ && b2.num.s==__zero__)
                {
                    //----------------------------------------------------------
                    //                  -->
                    // both vectors are nul : special case => true
                    //--------------------------------------------------------------
                    return true;
                }
                else
                {
                    assert(a2>0 || b2>0);
                    if(ab.num.s==__zero__)
                    {
                        //------------------------------------------------------
                        //               -->
                        // one vector is nul or orthogonality => false
                        //------------------------------------------------------
                        return false;
                    }
                    else
                    {
                        //------------------------------------------------------
                        //                  -->
                        // both vectors NOT nul : return b2 * alpha == ab * beta
                        //------------------------------------------------------
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




            //! direct projection of source on target
            template <typename SOURCE, typename TARGET> static inline
            void orthogonal_projection(writable<apq> &result, SOURCE &source, TARGET &target)
            {
                assert(source.size()>0);
                assert(source.size()==target.size());
                assert(result.size()==target.size());

                const apq fac = orthogonal_coefficient(source,target,"orthogonal projection on null target");
                for(size_t i=result.size();i>0;--i)
                {
                    result[i] = fac * target[i];
                }
            }

            //! compute orthogonal difference of source from target
            template <typename SOURCE, typename TARGET> static inline
            void orthogonal_difference(writable<apq> &result, SOURCE &source, TARGET &target)
            {
                assert(source.size()>0);
                assert(source.size()==target.size());
                assert(result.size()==target.size());

                const apq fac = orthogonal_coefficient(source,target,"orthogonal difference from null target");
                for(size_t i=result.size();i>0;--i)
                {
                    result[i] = source[i] - fac * target[i];
                }
                assert(0==internal::dot(result,target));
            }

            //______________________________________________________________________
            //
            //! rank by gauss jordan
            //______________________________________________________________________
            static size_t rank_of(matrix<apq> &Q);

            //______________________________________________________________________
            //
            //! rank by gauss jordan of a matrix converted to apq
            //______________________________________________________________________
            template <typename T> static inline
            size_t  rank(const matrix<T> &M)
            {
                matrix<apq> Q(M,transmogrify);
                assert(matrix_metrics::have_same_sizes(Q,M));
                return  rank_of(Q);
            }


        private:
            template <typename SOURCE, typename TARGET> static inline
            apq orthogonal_coefficient(SOURCE &source, TARGET &target, const char *msg)
            {
                assert(source.size()>0);
                assert(source.size()==target.size());
                apq          ts;
                apq          t2;
                for(size_t i=source.size();i>0;--i)
                {
                    typename TARGET::const_type &target_i = target[i];
                    internal::add_prod<typename TARGET::type, typename SOURCE::type>::to(ts, target_i, source[i]);
                    internal::add_mod2<typename TARGET::type>::to(t2, target_i);
                }
                if(__zero__ == t2.num.s) raise_error(msg);
                return ts/t2;
            }

        };

    }
}

#endif

