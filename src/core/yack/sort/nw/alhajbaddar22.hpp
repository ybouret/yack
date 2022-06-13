//! \file
#ifndef YACK_NWSORST_alhajbaddar22_INCLUDED
#define YACK_NWSORST_alhajbaddar22_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! alhajbaddar22
    class alhajbaddar22 : public pairs<22,0x0074> {
      public:
        virtual ~alhajbaddar22() throw(); //!< cleanup
        explicit alhajbaddar22() throw(); //!< setup  
        static const char   sid[];        //!< alhajbaddar22
        static const size_t lhs[0x0074];  //!< lhs
        static const size_t rhs[0x0074];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(alhajbaddar22);
    };
  }
}
#endif
