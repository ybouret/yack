//! \file
#ifndef YACK_NWSORST_hibbard24_INCLUDED
#define YACK_NWSORST_hibbard24_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard24
    class hibbard24 : public swaps {
      public:
        virtual ~hibbard24() noexcept; //!< cleanup
        explicit hibbard24() noexcept; //!< setup  
        static const char   sid[];        //!< hibbard24
        static const size_t lhs[0x008a];  //!< lhs
        static const size_t rhs[0x008a];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard24);
    };
  }
}
#endif
