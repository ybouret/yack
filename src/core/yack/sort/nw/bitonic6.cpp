#include "bitonic6.hpp"
namespace yack {
  namespace nwsrt {
    const char bitonic6 :: sid[] ="bitonic6";
    bitonic6 :: ~bitonic6() throw() {}
    bitonic6 ::  bitonic6() throw() : pairs<6,0x000d>(sid,lhs,rhs) {}
    const size_t bitonic6 :: lhs[0x000d] = {
 0x0002, 0x0005, 0x0001, 0x0004, 0x0002, 0x0004, 0x0003, 0x0002,
 0x0001, 0x0002, 0x0005, 0x0001, 0x0003
    };
    const size_t bitonic6 :: rhs[0x000d] = {
 0x0003, 0x0006, 0x0002, 0x0006, 0x0003, 0x0005, 0x0005, 0x0006,
 0x0003, 0x0004, 0x0006, 0x0002, 0x0004
    };
  }
}
