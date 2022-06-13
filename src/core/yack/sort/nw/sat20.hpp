//! \file
#ifndef YACK_NWSORST_sat20_INCLUDED
#define YACK_NWSORST_sat20_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! sat20
    class sat20 : public pairs<20,0x0066> {
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
