//! \file
#ifndef YACK_NWSORST_hibbard16_INCLUDED
#define YACK_NWSORST_hibbard16_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard16
    class hibbard16 : public swaps {
      public:
        virtual ~hibbard16() noexcept; //!< cleanup
        explicit hibbard16() noexcept; //!< setup  
        static const char   sid[];        //!< hibbard16
        static const size_t lhs[0x0041];  //!< lhs
        static const size_t rhs[0x0041];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard16);
    };
  }
}
#endif
