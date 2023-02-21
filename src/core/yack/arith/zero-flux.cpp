#include "yack/arith/zero-flux.hpp"

namespace yack
{

    namespace core
    {
        zero_flux::  zero_flux(unit_t n) noexcept :
        size(n),
        szsz(n+n)
        {
        }

        zero_flux:: ~zero_flux() noexcept
        {
            coerce(size)=0;
            coerce(szsz)=0;
        }

        zero_flux:: zero_flux(const zero_flux &zf) noexcept :
        size( zf.size ),
        szsz( zf.szsz )
        {
        }

        namespace
        {
            static inline
            unit_t raw1D(const zero_flux &, unit_t) noexcept
            {
                return 0;
            }

            static inline
            unit_t rawND(const zero_flux &self, unit_t indx) noexcept
            {
                if(indx<0)
                {
                    return rawND(self,-indx);
                }
                else
                {
                    if(indx>=self.size)
                    {
                        return rawND(self,self.szsz - (++indx) );
                    }
                    else
                    {
                        return indx;
                    }
                }
            }

        }

        zero_flux::proc zero_flux::raw_proc_for(const zero_flux &self) noexcept
        {
            if(self.size<=1) return raw1D; else return rawND;
        }

        namespace
        {
            static inline
            unit_t cxx1D(const zero_flux &, unit_t) noexcept
            {
                return 1;
            }

            static inline
            unit_t cxxND(const zero_flux &self, unit_t indx) noexcept
            {
                if(indx<=0)
                {
                    return cxxND(self,2-indx);
                }
                else
                {
                    if(indx>self.size)
                    {
                        return cxxND(self,self.szsz - indx);
                    }
                    else
                    {
                        return indx;
                    }
                }
            }
        }

        zero_flux::proc zero_flux::cxx_proc_for(const zero_flux &self) noexcept
        {
            if(self.size<=1) return cxx1D; else return cxxND;
        }

    }


}
