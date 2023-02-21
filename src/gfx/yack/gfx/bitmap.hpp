
//! \file

#ifndef YACK_GFX_BITMAP_INCLUDED
#define YACK_GFX_BITMAP_INCLUDED 1

#include "yack/ptr/arc.hpp"
#include "yack/type/args.hpp"
#include "yack/gfx/bitfield.hpp"
#include "yack/gfx/bitrows.hpp"


namespace yack
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! bitmap: shared bits and shared rows
        //
        //______________________________________________________________________
        class bitmap : public article, public metrics
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            explicit bitmap(const unit_t W, const unit_t H, const unit_t D);

            //! setup from user data
            explicit bitmap(bitfield    *F,
                            const unit_t W,
                            const unit_t H,
                            const unit_t D);

            //! shared copy
            bitmap(const bitmap &other) noexcept;
            
            //! cleanup
            virtual ~bitmap() noexcept;

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const bitrow & line(const unit_t j) const noexcept; //!< get scan line
            bitrow &       line(const unit_t j)       noexcept; //!< get scan line
            void           vflip()                    noexcept; //!< vertical flip


        protected:
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            arc_ptr<bitfield> data; //!< holds bits
            arc_ptr<bitrows>  rows; //!< structured rows

        private:
            YACK_DISABLE_ASSIGN(bitmap);
        };


     

      

    }

}

#endif

