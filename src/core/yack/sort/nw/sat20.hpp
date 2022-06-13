//! \file
#ifndef YACK_NWSORST_sat20_INCLUDED
#define YACK_NWSORST_sat20_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! sat20
    class sat20 : public swaps {
      public:
        virtual ~sat20() throw(); //!< cleanup
        explicit sat20() throw(); //!< setup  
        static const char   sid[];        //!< sat20
        static const size_t lhs[0x0066];  //!< lhs
        static const size_t rhs[0x0066];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(sat20);
    };
  }
}
#endif
