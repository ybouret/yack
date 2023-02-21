//! \file
#ifndef YACK_NWSORST_sat17_INCLUDED
#define YACK_NWSORST_sat17_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! sat17
    class sat17 : public swaps {
      public:
        virtual ~sat17() noexcept; //!< cleanup
        explicit sat17() noexcept; //!< setup  
        static const char   sid[];        //!< sat17
        static const size_t lhs[0x004f];  //!< lhs
        static const size_t rhs[0x004f];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(sat17);
    };
  }
}
#endif
