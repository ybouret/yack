//! \file
#ifndef YACK_NWSORST_alhajbaddar18_INCLUDED
#define YACK_NWSORST_alhajbaddar18_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! alhajbaddar18
    class alhajbaddar18 : public swaps {
      public:
        virtual ~alhajbaddar18() throw(); //!< cleanup
        explicit alhajbaddar18() throw(); //!< setup  
        static const char   sid[];        //!< alhajbaddar18
        static const size_t lhs[0x0054];  //!< lhs
        static const size_t rhs[0x0054];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(alhajbaddar18);
    };
  }
}
#endif
