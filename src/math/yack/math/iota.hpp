//! \file

#ifndef YACK_IOTA_INCLUDED
#define YACK_IOTA_INCLUDED 1

#include "yack/container/matrix.hpp"
#include "yack/math/adder.hpp"

namespace yack {

    namespace math
    {
        //______________________________________________________________________
        //
        //
        //! Inline Operations of Template Algebra
        //
        //______________________________________________________________________
        struct iota
        {

            //------------------------------------------------------------------
            //! target[1..target.size()] = source[1..target.size()]
            //------------------------------------------------------------------
            template <typename TARGET, typename SOURCE> static inline
            void load(TARGET &target, SOURCE &source)
            {
                assert(source.size()>=target.size());
                for(size_t i=target.size();i>0;--i) target[i] = static_cast< typename TARGET::type >( source[i] );
            }

            //------------------------------------------------------------------
            //! target[1..source.size()] = source[1..source.size()]
            //------------------------------------------------------------------
            template <typename TARGET, typename SOURCE> static inline
            void save(TARGET &target, SOURCE &source)
            {
                assert(source.size()<=target.size());
                for(size_t i=source.size();i>0;--i) target[i] = static_cast< typename TARGET::type >( source[i] );
            }

            //------------------------------------------------------------------
            //! target[1..target.size()] = lhs + rhs
            //------------------------------------------------------------------
            template <typename TARGET, typename LHS, typename RHS> static inline
            void add(TARGET &target,LHS &lhs, RHS &rhs)
            {
                assert(target.size()<=lhs.size());
                assert(target.size()<=rhs.size());
                for(size_t i=target.size();i>0;--i) target[i] = static_cast< typename TARGET::type >( lhs[i] )  + static_cast< typename TARGET::type >( rhs[i] );
            }

            //------------------------------------------------------------------
            //! target[1..target.size()] = lhs - rhs
            //------------------------------------------------------------------
            template <typename TARGET, typename LHS, typename RHS> static inline
            void sub(TARGET &target,LHS &lhs, RHS &rhs)
            {
                assert(target.size()<=lhs.size());
                assert(target.size()<=rhs.size());
                for(size_t i=target.size();i>0;--i) target[i] = static_cast< typename TARGET::type >( lhs[i])- static_cast< typename TARGET::type >( rhs[i] );
            }



            //------------------------------------------------------------------
            //! M = A*A'
            //------------------------------------------------------------------
            template <typename T, typename U> static inline
            void gram(matrix<T> &M, const matrix<U> &A)
            {
                assert(M.rows==A.rows);
                assert(M.is_square());
                const size_t nc = A.cols;
                const size_t nr = M.rows;
                for(size_t i=nr;i>0;--i)
                {
                    matrix_row<T>       &Mi = M[i];
                    const matrix_row<U> &Ai = A[i];
                    for(size_t j=nr;j>0;--j)
                    {
                        const matrix_row<U> &Aj = A[j];
                        T sum = 0;
                        for(size_t k=nc;k>0;--k) sum += Ai[k] * Aj[k];
                        Mi[j] = sum;
                    }
                }
            }

            template <typename T>
            struct dot
            {
                template <typename LHS, typename RHS> static inline
                T of(LHS &lhs, RHS &rhs, adder<T> &xadd)
                {
                    assert(lhs.size()<=rhs.size());

                    xadd.ldz();
                    for(size_t i=lhs.size();i>0;--i)
                    {
                        xadd.push( lhs[i]*rhs[i] );
                    }
                    return xadd.get();
                }

                template <typename LHS, typename RHS> static inline
                T of(LHS &lhs, RHS &rhs)
                {
                    assert(lhs.size()<=rhs.size());
                    assert(lhs.size()>0);

                    T res= lhs[1]*rhs[1];
                    for(size_t i=lhs.size();i>1;--i)
                    {
                        res += lhs[i]*rhs[i];
                    }
                    return res;
                }
            };

            template <typename T>
            struct mod2
            {
                template <typename LHS> static inline
                T of(LHS &lhs, adder<T> &xadd)
                {
                    xadd.ldz();
                    for(size_t i=lhs.size();i>0;--i)
                    {
                        xadd.push( mod2_of(lhs[i]) );
                    }
                    return xadd.get();
                }

                template <typename LHS> static inline
                T of(LHS &lhs )
                {
                    T res = 0;
                    for(size_t i=lhs.size();i>0;--i)
                    {
                        res += mod2_of(lhs[i]);
                    }
                    return res;
                }

                template <typename LHS, typename RHS> static inline
                T of(LHS &lhs, RHS &rhs, adder<T> &xadd)
                {
                    xadd.ldz();
                    for(size_t i=lhs.size();i>0;--i)
                    {
                        xadd.push( mod2_of(lhs[i]-rhs[i]) );
                    }
                    return xadd.get();
                }

                template <typename LHS, typename RHS> static inline
                T of(LHS &lhs, RHS &rhs)
                {
                    T res = 0;
                    for(size_t i=lhs.size();i>0;--i)
                    {
                        res += mod2_of(lhs[i]-rhs[i]);
                    }
                    return res;
                }


            };



            //------------------------------------------------------------------
            //! lhs = M * rhs
            //------------------------------------------------------------------
            template <typename T, typename LHS, typename RHS> static inline
            void mmul(LHS &lhs, const matrix<T> &M, RHS &rhs)
            {
                assert(lhs.size()>=M.rows);
                assert(rhs.size()>=M.cols);
                for(size_t i=M.rows;i>0;--i)
                    lhs[i] = dot<T>::of(M[i],rhs);
            }

            //------------------------------------------------------------------
            //! lhs = M * rhs
            //------------------------------------------------------------------
            template <typename T, typename LHS, typename RHS> static inline
            void mmul(LHS &lhs, const matrix<T> &M, RHS &rhs, adder<T> &xadd)
            {
                assert(lhs.size()>=M.rows);
                assert(rhs.size()>=M.cols);
                for(size_t i=M.rows;i>0;--i)
                    lhs[i] = dot<T>::of(M[i],rhs,xadd);
            }



        };

    }

}

#endif

