//! \file
#ifndef YACK_NWSORST_green16_INCLUDED
#define YACK_NWSORST_green16_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! green16
    class green16 : public swaps {
      public:
        virtual ~green16() noexcept; //!< cleanup
        explicit green16() noexcept; //!< setup  
        static const char   sid[];        //!< green16
        static const size_t lhs[0x003c];  //!< lhs
        static const size_t rhs[0x003c];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(green16);
    };
  }
}
#endif
