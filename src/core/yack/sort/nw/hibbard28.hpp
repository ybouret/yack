//! \file
#ifndef YACK_NWSORST_hibbard28_INCLUDED
#define YACK_NWSORST_hibbard28_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard28
    class hibbard28 : public pairs<28,0x00b3> {
      public:
        virtual ~hibbard28() throw(); //!< cleanup
        explicit hibbard28() throw(); //!< setup  
        static const char   sid[];        //!< hibbard28
        static const size_t lhs[0x00b3];  //!< lhs
        static const size_t rhs[0x00b3];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard28);
    };
  }
}
#endif
