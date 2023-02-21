//! \file
#ifndef YACK_NWSORST_hibbard12_INCLUDED
#define YACK_NWSORST_hibbard12_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard12
    class hibbard12 : public swaps {
      public:
        virtual ~hibbard12() noexcept; //!< cleanup
        explicit hibbard12() noexcept; //!< setup  
        static const char   sid[];        //!< hibbard12
        static const size_t lhs[0x002a];  //!< lhs
        static const size_t rhs[0x002a];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard12);
    };
  }
}
#endif
