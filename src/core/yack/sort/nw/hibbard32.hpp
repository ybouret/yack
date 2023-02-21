//! \file
#ifndef YACK_NWSORST_hibbard32_INCLUDED
#define YACK_NWSORST_hibbard32_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard32
    class hibbard32 : public swaps {
      public:
        virtual ~hibbard32() noexcept; //!< cleanup
        explicit hibbard32() noexcept; //!< setup  
        static const char   sid[];        //!< hibbard32
        static const size_t lhs[0x00d3];  //!< lhs
        static const size_t rhs[0x00d3];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard32);
    };
  }
}
#endif
