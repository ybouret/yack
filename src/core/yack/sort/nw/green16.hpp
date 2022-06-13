//! \file
#ifndef YACK_NWSORST_green16_INCLUDED
#define YACK_NWSORST_green16_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! green16
    class green16 : public pairs<16,0x003c> {
      public:
        virtual ~green16() throw(); //!< cleanup
        explicit green16() throw(); //!< setup  
        static const char   sid[];        //!< green16
        static const size_t lhs[0x003c];  //!< lhs
        static const size_t rhs[0x003c];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(green16);
    };
  }
}
#endif
