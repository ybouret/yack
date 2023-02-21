#include "bosenelson10.hpp"
namespace yack {
  namespace nwsrt {
    const char bosenelson10 :: sid[] ="bosenelson10";
    bosenelson10 :: ~bosenelson10() noexcept {}
    bosenelson10 ::  bosenelson10() noexcept : swaps(sid,10,0x0020,lhs,rhs) {}
    const size_t bosenelson10 :: lhs[0x0020] = {
 0x0001, 0x0004, 0x0006, 0x0009, 0x0003, 0x0008, 0x0003, 0x0002,
 0x0008, 0x0007, 0x0001, 0x0006, 0x0005, 0x0001, 0x0002, 0x0006,
 0x0007, 0x0002, 0x0007, 0x0001, 0x0004, 0x0002, 0x0003, 0x0005,
 0x0002, 0x0004, 0x0005, 0x0003, 0x0004, 0x0005, 0x0004, 0x0005
    };
    const size_t bosenelson10 :: rhs[0x0020] = {
 0x0002, 0x0005, 0x0007, 0x000a, 0x0005, 0x000a, 0x0004, 0x0005,
 0x0009, 0x000a, 0x0004, 0x0009, 0x000a, 0x0003, 0x0004, 0x0008,
 0x0009, 0x0003, 0x0008, 0x0006, 0x0009, 0x0007, 0x0008, 0x0009,
 0x0006, 0x0008, 0x0008, 0x0006, 0x0007, 0x0007, 0x0006, 0x0006
    };
  }
}
