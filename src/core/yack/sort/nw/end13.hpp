//! \file
#ifndef YACK_NWSORST_end13_INCLUDED
#define YACK_NWSORST_end13_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! end13
    class end13 : public swaps {
      public:
        virtual ~end13() noexcept; //!< cleanup
        explicit end13() noexcept; //!< setup  
        static const char   sid[];        //!< end13
        static const size_t lhs[0x002d];  //!< lhs
        static const size_t rhs[0x002d];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(end13);
    };
  }
}
#endif
