//! \file
#ifndef YACK_NWSORST_batcher26_INCLUDED
#define YACK_NWSORST_batcher26_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher26
    class batcher26 : public pairs<26,0x0092> {
      public:
        virtual ~batcher26() throw(); //!< cleanup
        explicit batcher26() throw(); //!< setup  
        static const char   sid[];        //!< batcher26
        static const size_t lhs[0x0092];  //!< lhs
        static const size_t rhs[0x0092];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher26);
    };
  }
}
#endif
