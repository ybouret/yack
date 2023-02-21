//! \file
#ifndef YACK_NWSORST_hibbard10_INCLUDED
#define YACK_NWSORST_hibbard10_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard10
    class hibbard10 : public swaps {
      public:
        virtual ~hibbard10() noexcept; //!< cleanup
        explicit hibbard10() noexcept; //!< setup  
        static const char   sid[];        //!< hibbard10
        static const size_t lhs[0x0020];  //!< lhs
        static const size_t rhs[0x0020];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard10);
    };
  }
}
#endif
