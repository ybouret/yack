//! \file
#ifndef YACK_NWSORST_hibbard31_INCLUDED
#define YACK_NWSORST_hibbard31_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard31
    class hibbard31 : public swaps {
      public:
        virtual ~hibbard31() noexcept; //!< cleanup
        explicit hibbard31() noexcept; //!< setup  
        static const char   sid[];        //!< hibbard31
        static const size_t lhs[0x00ce];  //!< lhs
        static const size_t rhs[0x00ce];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard31);
    };
  }
}
#endif
