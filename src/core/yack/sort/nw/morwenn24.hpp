//! \file
#ifndef YACK_NWSORST_morwenn24_INCLUDED
#define YACK_NWSORST_morwenn24_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! morwenn24
    class morwenn24 : public pairs<24,0x007b> {
      public:
        virtual ~morwenn24() throw(); //!< cleanup
        explicit morwenn24() throw(); //!< setup  
        static const char   sid[];        //!< morwenn24
        static const size_t lhs[0x007b];  //!< lhs
        static const size_t rhs[0x007b];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(morwenn24);
    };
  }
}
#endif
