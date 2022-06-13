//! \file
#ifndef YACK_NWSORST_hibbard29_INCLUDED
#define YACK_NWSORST_hibbard29_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard29
    class hibbard29 : public pairs<29,0x00bf> {
      public:
        virtual ~hibbard29() throw(); //!< cleanup
        explicit hibbard29() throw(); //!< setup  
        static const char   sid[];        //!< hibbard29
        static const size_t lhs[0x00bf];  //!< lhs
        static const size_t rhs[0x00bf];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard29);
    };
  }
}
#endif
