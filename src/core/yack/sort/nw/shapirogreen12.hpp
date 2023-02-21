//! \file
#ifndef YACK_NWSORST_shapirogreen12_INCLUDED
#define YACK_NWSORST_shapirogreen12_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! shapirogreen12
    class shapirogreen12 : public swaps {
      public:
        virtual ~shapirogreen12() noexcept; //!< cleanup
        explicit shapirogreen12() noexcept; //!< setup  
        static const char   sid[];        //!< shapirogreen12
        static const size_t lhs[0x0027];  //!< lhs
        static const size_t rhs[0x0027];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(shapirogreen12);
    };
  }
}
#endif
