//! \file
#ifndef YACK_NWSORST_hibbard11_INCLUDED
#define YACK_NWSORST_hibbard11_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard11
    class hibbard11 : public swaps {
      public:
        virtual ~hibbard11() noexcept; //!< cleanup
        explicit hibbard11() noexcept; //!< setup  
        static const char   sid[];        //!< hibbard11
        static const size_t lhs[0x0026];  //!< lhs
        static const size_t rhs[0x0026];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard11);
    };
  }
}
#endif
