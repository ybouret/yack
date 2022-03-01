
//! \file

#ifndef YACK_COLOR_YUV_INCLUDED
#define YACK_COLOR_YUV_INCLUDED 1

#include "yack/type/args.hpp"

namespace yack
{
    namespace color
    {

        namespace nexus
        {
#define YACK_YUV_UMIN -0.436   //!< decl
#define YACK_YUV_UMAX  0.436   //!< decl

#define YACK_YUV_VMIN -0.615   //!< decl
#define YACK_YUV_VMAX  0.615   //!< decl

            //__________________________________________________________________
            //
            //
            //! YUV format
            //
            //__________________________________________________________________
            template <typename T>
            class yuv
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                YACK_DECL_ARGS_(T,type); //!< aliases

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup
                yuv() throw();
                //! cleanup
                ~yuv() throw();
                //! copy
                yuv(const yuv & ) throw();
                //! assign
                yuv & operator=(const yuv &) throw();
                //! setup
                yuv(const_type Y, const_type U, const_type V) throw();

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const_type y; //!< Y
                const_type u; //!< U
                const_type v; //!< V


                //______________________________________________________________
                //
                // recomposing
                //______________________________________________________________



                inline mutable_type get_r() const throw()
                {
                    static const_type cv(1.13983);
                    return clamp_y(y+cv*v);
                }

                inline mutable_type get_g() const throw()
                {
                    static const_type cu(-0.39465);
                    static const_type cv(-0.58060);
                    return clamp_y(y+cu*u+cv*v);
                }

                inline mutable_type get_b() const throw()
                {
                    static const_type cu(2.03211);
                    return clamp_y(y+cu*u);
                }

                //______________________________________________________________
                //
                // clamping
                //______________________________________________________________

                //! Y
                static inline mutable_type clamp_y(const_type Y) throw()
                {
                    return (Y<=0) ? 0 : ( (1<=Y) ? 1 : Y);
                }

                //! U
                static inline mutable_type clamp_u(const_type U) throw()
                {
                    static const_type umin(YACK_YUV_UMIN);
                    static const_type umax(YACK_YUV_UMAX);

                    return (U<=umin) ? umin : ( (umax<=U) ? umax : U);
                }

                //! V
                static inline mutable_type clamp_v(const_type V) throw()
                {
                    static const_type vmin(YACK_YUV_VMIN);
                    static const_type vmax(YACK_YUV_VMAX);
                    
                    return (V<=vmin) ? vmin : ( (vmax<=V) ? vmax : V);
                }

                //______________________________________________________________
                //
                // conversion
                //______________________________________________________________

                //! Y
                static inline mutable_type to_y(const_type r, const_type g, const_type b) throw()
                {
                    static const_type cr(0.299);
                    static const_type cg(0.587);
                    static const_type cb(0.114);
                    return clamp_y(cr*r + cg*g + cb*b);
                }

                //! U
                static inline mutable_type to_u(const_type r, const_type g, const_type b) throw()
                {
                    static const_type cr(-0.14713);
                    static const_type cg(-0.28886);
                    static const_type cb(+0.43600);
                    return clamp_u(cr*r + cg*g + cb*b);
                }

                //! V
                static inline mutable_type to_v(const_type r, const_type g, const_type b) throw()
                {
                    static const_type cr(+0.61500);
                    static const_type cg(-0.51498);
                    static const_type cb(-0.10001);
                    return clamp_v(cr*r + cg*g + cb*b);
                }

                //! [0..1]^3 to yuv
                static yuv from_reals(const_type r, const_type g, const_type b) throw();

                //! [0..255]^3 to yuv
                static yuv from_bytes(const uint8_t r, const uint8_t g, const uint8_t b) throw();

            };

        }


        typedef nexus::yuv<float> yuv;
    }

}

#endif
