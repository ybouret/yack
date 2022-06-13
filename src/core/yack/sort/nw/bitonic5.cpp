#include "bitonic5.hpp"
namespace yack {
  namespace nwsrt {
    const char bitonic5 :: sid[] ="bitonic5";
    bitonic5 :: ~bitonic5() throw() {}
    bitonic5 ::  bitonic5() throw() : pairs<5,0x0009>(sid,lhs,rhs) {}
    const size_t bitonic5 :: lhs[0x0009] = {
 0x0001, 0x0004, 0x0003, 0x0003, 0x0002, 0x0002, 0x0001, 0x0001,
 0x0003
    };
    const size_t bitonic5 :: rhs[0x0009] = {
 0x0002, 0x0005, 0x0005, 0x0004, 0x0005, 0x0003, 0x0004, 0x0002,
 0x0004
    };
  }
}
