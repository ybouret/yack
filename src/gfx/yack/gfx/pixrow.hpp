//! \file

#ifndef YACK_GFX_PIXROW_INCLUDED
#define YACK_GFX_PIXROW_INCLUDED 1

#include "yack/gfx/zero-flux.hpp"
#include "yack/type/args.hpp"

namespace yack
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! mapping bitrow ABI
        //
        //______________________________________________________________________
        template <typename T>
        class pixrow
        {
        public:
            YACK_DECL_ARGS_(T,type); //!< aliases

            //! direct access
            inline type & operator()(const unit_t x) throw() {
                assert(x>=0); assert(x<*w); assert(0!=p);
                return p[x];
            }

            //! direct CONSST access
            inline const_type & operator()(const unit_t x) const throw() {
                assert(x>=0); assert(x<*w); assert(0!=p);
                return p[x];
            }

            //! zero-flux access
            inline type & operator[](const unit_t x) throw() {
                return p[ w(x) ];
            }

            //! zero-flux CONST access
            inline const_type & operator[](const unit_t x) const throw() {
                return p[ w(x) ];
            }

        private: T               *p; //!< first item address
        public:  const zero_flux &w; //!< zero flux computation reference

        private:
            pixrow() throw();
            ~pixrow();
            YACK_DISABLE_COPY_AND_ASSIGN(pixrow);
        };
    }

}

#endif
