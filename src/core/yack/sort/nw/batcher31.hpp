//! \file
#ifndef YACK_NWSORST_batcher31_INCLUDED
#define YACK_NWSORST_batcher31_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher31
    class batcher31 : public pairs<31,0x00ba> {
      public:
        virtual ~batcher31() throw(); //!< cleanup
        explicit batcher31() throw(); //!< setup  
        static const char   sid[];        //!< batcher31
        static const size_t lhs[0x00ba];  //!< lhs
        static const size_t rhs[0x00ba];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher31);
    };
  }
}
#endif
