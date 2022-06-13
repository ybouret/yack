//! \file
#ifndef YACK_NWSORST_batcher4_INCLUDED
#define YACK_NWSORST_batcher4_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher4
    class batcher4 : public pairs<4,0x0005> {
      public:
        virtual ~batcher4() throw(); //!< cleanup
        explicit batcher4() throw(); //!< setup  
        static const char   sid[];        //!< batcher4
        static const size_t lhs[0x0005];  //!< lhs
        static const size_t rhs[0x0005];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher4);
    };
  }
}
#endif
