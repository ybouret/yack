#include "bosenelson2.hpp"
namespace yack {
  namespace nwsrt {
    const char bosenelson2 :: sid[] ="bosenelson2";
    bosenelson2 :: ~bosenelson2() throw() {}
    bosenelson2 ::  bosenelson2() throw() : pairs<2,0x0001>(sid,lhs,rhs) {}
    const size_t bosenelson2 :: lhs[0x0001] = {
 0x0001
    };
    const size_t bosenelson2 :: rhs[0x0001] = {
 0x0002
    };
  }
}
