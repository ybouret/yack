#include "bosenelson6.hpp"
namespace yack {
  namespace nwsrt {
    const char bosenelson6 :: sid[] ="bosenelson6";
    bosenelson6 :: ~bosenelson6() throw() {}
    bosenelson6 ::  bosenelson6() throw() : swaps(sid,6,0x000c,lhs,rhs) {}
    const size_t bosenelson6 :: lhs[0x000c] = {
 0x0002, 0x0005, 0x0001, 0x0004, 0x0001, 0x0004, 0x0003, 0x0001,
 0x0002, 0x0003, 0x0002, 0x0003
    };
    const size_t bosenelson6 :: rhs[0x000c] = {
 0x0003, 0x0006, 0x0003, 0x0006, 0x0002, 0x0005, 0x0006, 0x0004,
 0x0005, 0x0005, 0x0004, 0x0004
    };
  }
}
