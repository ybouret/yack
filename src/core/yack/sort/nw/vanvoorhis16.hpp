//! \file
#ifndef YACK_NWSORST_vanvoorhis16_INCLUDED
#define YACK_NWSORST_vanvoorhis16_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! vanvoorhis16
    class vanvoorhis16 : public swaps {
      public:
        virtual ~vanvoorhis16() noexcept; //!< cleanup
        explicit vanvoorhis16() noexcept; //!< setup  
        static const char   sid[];        //!< vanvoorhis16
        static const size_t lhs[0x003d];  //!< lhs
        static const size_t rhs[0x003d];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(vanvoorhis16);
    };
  }
}
#endif
