#include "shapirogreen12.hpp"
namespace yack {
  namespace nwsrt {
    const char shapirogreen12 :: sid[] ="shapirogreen12";
    shapirogreen12 :: ~shapirogreen12() throw() {}
    shapirogreen12 ::  shapirogreen12() throw() : pairs<12,0x0027>(sid,lhs,rhs) {}
    const size_t shapirogreen12 :: lhs[0x0027] = {
 0x0001, 0x0003, 0x0005, 0x0007, 0x0009, 0x000b, 0x0002, 0x0006,
 0x000a, 0x0001, 0x0005, 0x0009, 0x0002, 0x0006, 0x000a, 0x0001,
 0x0008, 0x0002, 0x0007, 0x0004, 0x0005, 0x0006, 0x0003, 0x0001,
 0x0008, 0x0004, 0x0002, 0x0007, 0x0003, 0x0009, 0x0002, 0x0008,
 0x0004, 0x0007, 0x0003, 0x0008, 0x0006, 0x0004, 0x0008
    };
    const size_t shapirogreen12 :: rhs[0x0027] = {
 0x0002, 0x0004, 0x0006, 0x0008, 0x000a, 0x000c, 0x0004, 0x0008,
 0x000c, 0x0003, 0x0007, 0x000b, 0x0003, 0x0007, 0x000b, 0x0005,
 0x000c, 0x0006, 0x000b, 0x0008, 0x0009, 0x000a, 0x0007, 0x0005,
 0x000c, 0x0009, 0x0006, 0x000b, 0x0004, 0x000a, 0x0005, 0x000b,
 0x0006, 0x0009, 0x0005, 0x000a, 0x0007, 0x0005, 0x0009
    };
  }
}
