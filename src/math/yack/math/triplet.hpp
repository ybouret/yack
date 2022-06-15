//! \file

#ifndef YACK_TRIPLET_INCLUDED
#define YACK_TRIPLET_INCLUDED 1

#include "yack/type/args.hpp"
#include "yack/sort/heap.hpp"
#include "yack/comparison.hpp"
#include <iostream>

namespace yack
{

    namespace math
    {

        //______________________________________________________________________
        //
        //
        //! multipurpose triplet
        //
        //______________________________________________________________________
        template <typename T>
        struct triplet
        {

            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            YACK_DECL_ARGS_(T,type); //!< aliases


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            T a; //!< value
            T b; //!< value
            T c; //!< value

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! array like interface
            inline size_t size() const throw() { return 3; }

            //! array like interface
            inline type  &operator[](const size_t indx) throw()
            {
                assert(indx>=1); assert(indx<=3);
                return *((((mutable_type *)&a)-1)+indx);
            }

            //! array like interface
            inline const_type &operator[](const size_t indx) const throw()
            {
                assert(indx>=1); assert(indx<=3);
                return *((((const_type *)&a)-1)+indx);
            }

            //! sort in increasing order
            inline void sort() throw()
            {
                hsort(*this,comparison::increasing<T>);
            }

            //! co-sort in increasing order
            template <typename U>
            inline void sort( triplet<U> &other ) throw()
            {
                hsort(*this,other,comparison::increasing<T>);
            }

            //! assign by static_cast
            template <typename U>
            inline void assign(const triplet<U> &other)
            {
                a = static_cast<const_type>(other.a);
                b = static_cast<const_type>(other.b);
                c = static_cast<const_type>(other.c);
            }

            //! (a,b,c) -> arr[0..2]
            template <typename U>
            inline void save(U arr[]) const
            {
                arr[0] = static_cast<U>(a);
                arr[1] = static_cast<U>(b);
                arr[2] = static_cast<U>(c);
            }

            //! check if is increasing
            inline bool is_increasing() const throw()
            {
                return comparison::ordered(&a,3,comparison::increasing<T>);
            }

            //! check if is decreasing
            inline bool is_decreasing() const throw()
            {
                return comparison::ordered(&a,3,comparison::decreasing<T>);
            }

            //! check if is ordered
            inline bool is_ordered() const throw()
            {
                return is_increasing() || is_decreasing();
            }

            //! check if is local minimum
            inline bool is_local_minimum() const throw()
            {
                return (a>=b) && (c>=b);
            }

            //! (a,b,c) -> (c,b,a)
            inline void reverse() throw()
            {
                cswap(a,c);
            }

            //! output as a 3-array
            friend inline
            std::ostream & operator<<(std::ostream &os, const triplet &t)
            {
                os << '[' << t.a << ';' << t.b << ';' << t.c << ']';
                return os;
            }


            //! return minimum value
            inline T mini() const throw()
            {
                T ans = a;
                if(b<ans) ans=b;
                if(c<ans) ans=c;
                return ans;
            }

            //! return maximum value
            inline T maxi() const throw()
            {
                T ans = a;
                if(ans<b) ans=b;
                if(ans<c) ans=c;
                return ans;
            }

            //! return amplitude
            inline T amplitude() const throw()
            {
                T lo=a, hi=lo;
                if(b<lo)
                {
                    lo=b;
                }
                else
                {
                    if(hi<b)
                        hi=b;
                }

                if(c<lo)
                {
                    lo=c;
                }
                else
                {
                    if(hi<c)
                        hi=c;
                }

                return hi-lo;
            }

            //! return max(|a|,|b|,|c|)
            inline T maxabs() const throw()
            {
                T ans = absolute(a);
                { const T tmp = absolute(b); if(ans<tmp) ans=tmp; }
                { const T tmp = absolute(c); if(ans<tmp) ans=tmp; }
                return ans;
            }

            //! return min(|a|,|b|,|c|)
            inline T minabs() const throw()
            {
                T ans = absolute(a);
                { const T tmp = absolute(b); if(tmp<ans) ans=tmp; }
                { const T tmp = absolute(c); if(tmp<ans) ans=tmp; }
                return ans;
            }

        };

    }

}

#endif

