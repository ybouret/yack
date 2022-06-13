//! \file
#ifndef YACK_NWSORST_hibbard4_INCLUDED
#define YACK_NWSORST_hibbard4_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard4
    class hibbard4 : public pairs<4,0x0005> {
      public:
        virtual ~hibbard4() throw(); //!< cleanup
        explicit hibbard4() throw(); //!< setup  
        static const char   sid[];        //!< hibbard4
        static const size_t lhs[0x0005];  //!< lhs
        static const size_t rhs[0x0005];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard4);
    };
  }
}
#endif
