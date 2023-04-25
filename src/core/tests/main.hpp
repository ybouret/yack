//! \file
//!
#ifndef MAIN_INCLUDED
#define MAIN_INCLUDED 1

#include "yack/randomized/bits.hpp"
#include "yack/randomized/in2d.hpp"
#include "yack/randomized/in3d.hpp"
#include "yack/apex.hpp"
#include "yack/container/matrix.hpp"
#include "yack/type/complex.hpp"
#include "yack/type/v3d.hpp"
#include "yack/string.hpp"
#include "yack/color/rgba.hpp"
#include "yack/math/xcomplex.hpp"


namespace yack
{
    struct bring
    {
        static const size_t apex_bits=20;
        
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

    // char
    template <>
    inline char bring:: get<char>(randomized::bits &ran)
    {
        return char(ran.in('a','z'));
    }

    // int
    template < >
    inline int bring:: get<int>(randomized::bits &ran)
    {
        return int(ran.in(-100,100));
    }

    // short
    template < >
    inline short int bring:: get<short int>(randomized::bits &ran)
    {
        return static_cast<short int>(ran.in(-100,100));
    }


    // unsigned
    template < >
    inline unsigned bring:: get<unsigned>(randomized::bits &ran)
    {
        return unsigned(ran.in(0,100));
    }


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

    // extended floating point
    template <>
    inline extended<float> bring:: get< extended<float> >(randomized::bits &ran)
    {
        return ran.to<float>();
    }

    template <>
    inline extended<double> bring:: get< extended<double> >(randomized::bits &ran)
    {
        return ran.to<double>();
    }

    template <>
    inline extended<long double> bring:: get< extended<long double> >(randomized::bits &ran)
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

    // xcomplexes
    template <>
    inline xcomplex<float> bring:: get< xcomplex<float> >(randomized::bits &ran)
    {
        return randomized::in2d::in_disk<float,complex>(ran);
    }

    template <>
    inline xcomplex<double> bring:: get< xcomplex<double> >(randomized::bits &ran)
    {
        return randomized::in2d::in_disk<double,complex>(ran);
    }

    template <>
    inline xcomplex<long double> bring:: get< xcomplex<long double> >(randomized::bits &ran)
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
        return apq(ran,apex_bits/2,apex_bits/2);
    }

    // V2D
    template <>
    inline v2d<float> bring:: get< v2d<float> >(randomized::bits &ran)
    {
        return randomized::in2d::on_circle<float,v2d>(ran);
    }

    template <>
    inline v2d<double> bring:: get< v2d<double> >(randomized::bits &ran)
    {
        return randomized::in2d::in_disk<double,v2d>(ran);
    }

    template <>
    inline v2d<long double> bring:: get< v2d<long double> >(randomized::bits &ran)
    {
        return randomized::in2d::in_disk<long double,v2d>(ran);
    }

    // V3D
    template <>
    inline v3d<float> bring:: get< v3d<float> >(randomized::bits &ran)
    {
        return randomized::in3d::in_ball<float,v3d>(ran);
    }

    template <>
    inline v3d<double> bring:: get< v3d<double> >(randomized::bits &ran)
    {
        return randomized::in3d::in_ball<double,v3d>(ran);
    }

    template <>
    inline v3d<long double> bring:: get< v3d<long double> >(randomized::bits &ran)
    {
        return randomized::in3d::in_ball<long double,v3d>(ran);
    }

    // string
    template <>
    inline string bring:: get< string >( randomized::bits &ran )
    {
        const size_t n = 1+ran.leq(15);
        string ans(n,as_capacity);
        for(size_t i=0;i<n;++i)
        {
            ans += ran.in('a','z');
        }
        return ans;;
    }

    //! rgb
    template <>
    inline color::rgb bring::get<color::rgb>( randomized::bits &ran )
    {
        return color::rgb( ran.to<uint8_t>(), ran.to<uint8_t>(), ran.to<uint8_t>() );
    }

    //! rgba
    template <>
    inline color::rgba bring::get<color::rgba>( randomized::bits &ran )
    {
        return color::rgba( ran.to<uint8_t>(), ran.to<uint8_t>(), ran.to<uint8_t>(), uint8_t(128+ran.in(0,127)) );
    }

}

#endif
