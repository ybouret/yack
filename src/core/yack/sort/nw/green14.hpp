//! \file
#ifndef YACK_NWSORST_green14_INCLUDED
#define YACK_NWSORST_green14_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! green14
    class green14 : public swaps {
      public:
        virtual ~green14() noexcept; //!< cleanup
        explicit green14() noexcept; //!< setup  
        static const char   sid[];        //!< green14
        static const size_t lhs[0x0033];  //!< lhs
        static const size_t rhs[0x0033];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(green14);
    };
  }
}
#endif
