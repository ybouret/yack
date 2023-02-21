//! file

#ifndef YACK_COLOR_CONVERT_INCLUDED
#define YACK_COLOR_CONVERT_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace color {

        //----------------------------------------------------------------------
        //
        //! conversion tables
        //
        //----------------------------------------------------------------------
        struct convtable
        {
            static const float   f[3*255+1]; //!< table of float
            static const uint8_t u[3*255+1]; //!< table of bytes
        };

        //----------------------------------------------------------------------
        //
        //! conversion prototype
        //
        //----------------------------------------------------------------------
        template <typename TARGET, typename SOURCE>
        struct convert
        {
            static void          make(TARGET &, const SOURCE &) noexcept; //!< SOURCE->TARGET
            static inline TARGET cast(const SOURCE  &src)       noexcept
            {
                TARGET tgt(0);
                make(tgt,src);
                return tgt;
            } //!< helper
        };

        //----------------------------------------------------------------------
        //
        //! specific case
        //
        //----------------------------------------------------------------------
        template <typename TARGET>
        struct convert<TARGET,TARGET>
        {
            static inline void   make(TARGET &tgt, const TARGET &src) noexcept { tgt=src;    } //!< SOURCE->TARGET
            static inline TARGET cast(const TARGET &src)              noexcept { return src; } //!< helper
        };

        

        //! wrapper for saveBMP essentially
        template <typename TARGET, typename SOURCE> inline
        TARGET converting(const void *addr) noexcept
        {
            assert(NULL!=addr);
            return convert<TARGET,SOURCE>::cast(*static_cast<const SOURCE *>(addr));
        }


    }
}


#endif

