#include "bitonic3.hpp"
namespace yack {
  namespace nwsrt {
    const char bitonic3 :: sid[] ="bitonic3";
    bitonic3 :: ~bitonic3() throw() {}
    bitonic3 ::  bitonic3() throw() : pairs<3,0x0003>(sid,lhs,rhs) {}
    const size_t bitonic3 :: lhs[0x0003] = {
 0x0002, 0x0001, 0x0001
    };
    const size_t bitonic3 :: rhs[0x0003] = {
 0x0003, 0x0003, 0x0002
    };
  }
}
