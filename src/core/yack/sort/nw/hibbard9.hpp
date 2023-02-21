//! \file
#ifndef YACK_NWSORST_hibbard9_INCLUDED
#define YACK_NWSORST_hibbard9_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard9
    class hibbard9 : public swaps {
      public:
        virtual ~hibbard9() noexcept; //!< cleanup
        explicit hibbard9() noexcept; //!< setup  
        static const char   sid[];        //!< hibbard9
        static const size_t lhs[0x001b];  //!< lhs
        static const size_t rhs[0x001b];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard9);
    };
  }
}
#endif
