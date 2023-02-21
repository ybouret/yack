//! \file
#ifndef YACK_NWSORST_hibbard20_INCLUDED
#define YACK_NWSORST_hibbard20_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard20
    class hibbard20 : public swaps {
      public:
        virtual ~hibbard20() noexcept; //!< cleanup
        explicit hibbard20() noexcept; //!< setup  
        static const char   sid[];        //!< hibbard20
        static const size_t lhs[0x006a];  //!< lhs
        static const size_t rhs[0x006a];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard20);
    };
  }
}
#endif
