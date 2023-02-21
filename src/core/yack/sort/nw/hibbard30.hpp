//! \file
#ifndef YACK_NWSORST_hibbard30_INCLUDED
#define YACK_NWSORST_hibbard30_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard30
    class hibbard30 : public swaps {
      public:
        virtual ~hibbard30() noexcept; //!< cleanup
        explicit hibbard30() noexcept; //!< setup  
        static const char   sid[];        //!< hibbard30
        static const size_t lhs[0x00c6];  //!< lhs
        static const size_t rhs[0x00c6];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard30);
    };
  }
}
#endif
