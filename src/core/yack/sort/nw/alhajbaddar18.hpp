//! \file
#ifndef YACK_NWSORST_alhajbaddar18_INCLUDED
#define YACK_NWSORST_alhajbaddar18_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! alhajbaddar18
    class alhajbaddar18 : public pairs<18,0x0054> {
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
