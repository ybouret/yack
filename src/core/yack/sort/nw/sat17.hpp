//! \file
#ifndef YACK_NWSORST_sat17_INCLUDED
#define YACK_NWSORST_sat17_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! sat17
    class sat17 : public pairs<17,0x004f> {
      public:
        virtual ~sat17() throw(); //!< cleanup
        explicit sat17() throw(); //!< setup  
        static const char   sid[];        //!< sat17
        static const size_t lhs[0x004f];  //!< lhs
        static const size_t rhs[0x004f];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(sat17);
    };
  }
}
#endif
