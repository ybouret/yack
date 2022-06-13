#include "bosenelson4.hpp"
namespace yack {
  namespace nwsrt {
    const char bosenelson4 :: sid[] ="bosenelson4";
    bosenelson4 :: ~bosenelson4() throw() {}
    bosenelson4 ::  bosenelson4() throw() : pairs<4,0x0005>(sid,lhs,rhs) {}
    const size_t bosenelson4 :: lhs[0x0005] = {
 0x0001, 0x0003, 0x0001, 0x0002, 0x0002
    };
    const size_t bosenelson4 :: rhs[0x0005] = {
 0x0002, 0x0004, 0x0003, 0x0004, 0x0003
    };
  }
}
