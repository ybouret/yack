//! \file
#ifndef YACK_NWSORST_hibbard18_INCLUDED
#define YACK_NWSORST_hibbard18_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard18
    class hibbard18 : public swaps {
      public:
        virtual ~hibbard18() noexcept; //!< cleanup
        explicit hibbard18() noexcept; //!< setup  
        static const char   sid[];        //!< hibbard18
        static const size_t lhs[0x005a];  //!< lhs
        static const size_t rhs[0x005a];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard18);
    };
  }
}
#endif
