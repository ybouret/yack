//! \file
#ifndef YACK_NWSORST_hibbard21_INCLUDED
#define YACK_NWSORST_hibbard21_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard21
    class hibbard21 : public swaps {
      public:
        virtual ~hibbard21() noexcept; //!< cleanup
        explicit hibbard21() noexcept; //!< setup  
        static const char   sid[];        //!< hibbard21
        static const size_t lhs[0x0076];  //!< lhs
        static const size_t rhs[0x0076];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard21);
    };
  }
}
#endif
