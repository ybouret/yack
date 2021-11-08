//! \file
//!
#ifndef MAIN_INCLUDED
#define MAIN_INCLUDED 1

#include "yack/randomized/bits.hpp"
#include "yack/randomized/in2d.hpp"
#include "yack/apex.hpp"
#include "yack/container/matrix.hpp"


namespace yack
{
    struct bring
    {
        static const size_t apex_bits=40;
        
        template <typename T>
        static T get(randomized::bits &);
        
        template <typename T> static inline
        void fill( writable<T> &arr, randomized::bits & ran)
        {
            for(size_t i=arr.size();i>0;--i)
            {
                arr[i] = get<T>(ran);
            }
        }
        
        
        template <typename T> static inline
        void fill(matrix<T> &a, randomized::bits &ran)
        {
            thin_array<T> arr = a.get_contiguous();
            fill(arr,ran);
        }
        
        
    };
    
    // floating point
    template <>
    inline float bring:: get<float>(randomized::bits &ran)
    {
        return ran.to<float>();
    }
    
    template <>
    inline double bring:: get<double>(randomized::bits &ran)
    {
        return ran.to<double>();
    }
    
    template <>
    inline long double bring:: get<long double>(randomized::bits &ran)
    {
        return ran.to<long double>();
    }
    
    // complexes
    template <>
    inline complex<float> bring:: get< complex<float> >(randomized::bits &ran)
    {
        return randomized::in2d::in_disk<float,complex>(ran);
    }
    
    template <>
    inline complex<double> bring:: get< complex<double> >(randomized::bits &ran)
    {
        return randomized::in2d::in_disk<double,complex>(ran);
    }
    
    template <>
    inline complex<long double> bring:: get< complex<long double> >(randomized::bits &ran)
    {
        return randomized::in2d::in_disk<long double,complex>(ran);
    }
    
    
    // apn
    template <>
    inline apn bring:: get<apn>(randomized::bits &ran)
    {
        return apn(ran,apex_bits);
    }
    
    // apz
    template <>
    inline apz bring:: get<apz>(randomized::bits &ran)
    {
        return apz(ran,apex_bits);
    }
    
    // apq
    template <>
    inline apq bring:: get<apq>(randomized::bits &ran)
    {
        return apq(ran,apex_bits,10);
    }
    
    
}

#endif
