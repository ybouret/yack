#include "bosenelson7.hpp"
namespace yack {
  namespace nwsrt {
    const char bosenelson7 :: sid[] ="bosenelson7";
    bosenelson7 :: ~bosenelson7() throw() {}
    bosenelson7 ::  bosenelson7() throw() : swaps(sid,7,0x0010,lhs,rhs) {}
    const size_t bosenelson7 :: lhs[0x0010] = {
 0x0002, 0x0004, 0x0006, 0x0001, 0x0004, 0x0005, 0x0001, 0x0005,
 0x0003, 0x0001, 0x0002, 0x0001, 0x0003, 0x0002, 0x0003, 0x0003
    };
    const size_t bosenelson7 :: rhs[0x0010] = {
 0x0003, 0x0005, 0x0007, 0x0003, 0x0006, 0x0007, 0x0002, 0x0006,
 0x0007, 0x0005, 0x0006, 0x0004, 0x0006, 0x0004, 0x0005, 0x0004
    };
  }
}
