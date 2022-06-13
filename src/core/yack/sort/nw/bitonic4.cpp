#include "bitonic4.hpp"
namespace yack {
  namespace nwsrt {
    const char bitonic4 :: sid[] ="bitonic4";
    bitonic4 :: ~bitonic4() throw() {}
    bitonic4 ::  bitonic4() throw() : pairs<4,0x0006>(sid,lhs,rhs) {}
    const size_t bitonic4 :: lhs[0x0006] = {
 0x0001, 0x0003, 0x0002, 0x0001, 0x0001, 0x0003
    };
    const size_t bitonic4 :: rhs[0x0006] = {
 0x0002, 0x0004, 0x0003, 0x0004, 0x0002, 0x0004
    };
  }
}
