//! \file
#ifndef YACK_NWSORST_hibbard4_INCLUDED
#define YACK_NWSORST_hibbard4_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard4
    class hibbard4 : public swaps {
      public:
        virtual ~hibbard4() noexcept; //!< cleanup
        explicit hibbard4() noexcept; //!< setup  
        static const char   sid[];        //!< hibbard4
        static const size_t lhs[0x0005];  //!< lhs
        static const size_t rhs[0x0005];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard4);
    };
  }
}
#endif
