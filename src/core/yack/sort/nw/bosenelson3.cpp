#include "bosenelson3.hpp"
namespace yack {
  namespace nwsrt {
    const char bosenelson3 :: sid[] ="bosenelson3";
    bosenelson3 :: ~bosenelson3() throw() {}
    bosenelson3 ::  bosenelson3() throw() : pairs<3,0x0003>(sid,lhs,rhs) {}
    const size_t bosenelson3 :: lhs[0x0003] = {
 0x0002, 0x0001, 0x0001
    };
    const size_t bosenelson3 :: rhs[0x0003] = {
 0x0003, 0x0003, 0x0002
    };
  }
}
