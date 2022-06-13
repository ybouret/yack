//! \file
#ifndef YACK_NWSORST_morwenn23_INCLUDED
#define YACK_NWSORST_morwenn23_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! morwenn23
    class morwenn23 : public swaps {
      public:
        virtual ~morwenn23() throw(); //!< cleanup
        explicit morwenn23() throw(); //!< setup  
        static const char   sid[];        //!< morwenn23
        static const size_t lhs[0x0076];  //!< lhs
        static const size_t rhs[0x0076];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(morwenn23);
    };
  }
}
#endif
