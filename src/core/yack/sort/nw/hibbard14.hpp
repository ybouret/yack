//! \file
#ifndef YACK_NWSORST_hibbard14_INCLUDED
#define YACK_NWSORST_hibbard14_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard14
    class hibbard14 : public swaps {
      public:
        virtual ~hibbard14() noexcept; //!< cleanup
        explicit hibbard14() noexcept; //!< setup  
        static const char   sid[];        //!< hibbard14
        static const size_t lhs[0x0037];  //!< lhs
        static const size_t rhs[0x0037];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard14);
    };
  }
}
#endif
