//! \file
#ifndef YACK_NWSORST_green14_INCLUDED
#define YACK_NWSORST_green14_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! green14
    class green14 : public pairs<14,0x0033> {
      public:
        virtual ~green14() throw(); //!< cleanup
        explicit green14() throw(); //!< setup  
        static const char   sid[];        //!< green14
        static const size_t lhs[0x0033];  //!< lhs
        static const size_t rhs[0x0033];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(green14);
    };
  }
}
#endif
