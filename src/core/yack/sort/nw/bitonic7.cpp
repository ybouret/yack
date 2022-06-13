#include "bitonic7.hpp"
namespace yack {
  namespace nwsrt {
    const char bitonic7 :: sid[] ="bitonic7";
    bitonic7 :: ~bitonic7() throw() {}
    bitonic7 ::  bitonic7() throw() : pairs<7,0x0012>(sid,lhs,rhs) {}
    const size_t bitonic7 :: lhs[0x0012] = {
 0x0002, 0x0004, 0x0006, 0x0001, 0x0005, 0x0004, 0x0002, 0x0004,
 0x0006, 0x0003, 0x0002, 0x0001, 0x0001, 0x0002, 0x0005, 0x0001,
 0x0003, 0x0005
    };
    const size_t bitonic7 :: rhs[0x0012] = {
 0x0003, 0x0005, 0x0007, 0x0002, 0x0006, 0x0007, 0x0003, 0x0005,
 0x0007, 0x0005, 0x0006, 0x0007, 0x0003, 0x0004, 0x0007, 0x0002,
 0x0004, 0x0006
    };
  }
}
