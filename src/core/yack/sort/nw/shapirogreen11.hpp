//! \file
#ifndef YACK_NWSORST_shapirogreen11_INCLUDED
#define YACK_NWSORST_shapirogreen11_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! shapirogreen11
    class shapirogreen11 : public swaps {
      public:
        virtual ~shapirogreen11() noexcept; //!< cleanup
        explicit shapirogreen11() noexcept; //!< setup  
        static const char   sid[];        //!< shapirogreen11
        static const size_t lhs[0x0023];  //!< lhs
        static const size_t rhs[0x0023];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(shapirogreen11);
    };
  }
}
#endif
