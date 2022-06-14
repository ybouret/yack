#include "bitonic16.hpp"
namespace yack {
  namespace nwsrt {
    const char bitonic16 :: sid[] ="bitonic16";
    bitonic16 :: ~bitonic16() throw() {}
    bitonic16 ::  bitonic16() throw() : swaps(sid,16,0x0050,lhs,rhs) {}
    const size_t bitonic16 :: lhs[0x0050] = {
 0x0001, 0x0003, 0x0005, 0x0007, 0x0009, 0x000b, 0x000d, 0x000f,
 0x0002, 0x0001, 0x0005, 0x0006, 0x0009, 0x000a, 0x000e, 0x000d,
 0x0001, 0x0003, 0x0007, 0x0005, 0x000b, 0x0009, 0x000d, 0x000f,
 0x0001, 0x0002, 0x0003, 0x0004, 0x000c, 0x000b, 0x000a, 0x0009,
 0x0006, 0x0005, 0x0001, 0x0002, 0x000a, 0x0009, 0x000d, 0x000e,
 0x0007, 0x0005, 0x0003, 0x0001, 0x0009, 0x000b, 0x000d, 0x000f,
 0x0008, 0x0007, 0x0006, 0x0005, 0x0004, 0x0003, 0x0002, 0x0001,
 0x0004, 0x0003, 0x0002, 0x0001, 0x0009, 0x000a, 0x000b, 0x000c,
 0x0002, 0x0001, 0x0006, 0x0005, 0x0009, 0x000a, 0x000d, 0x000e,
 0x0001, 0x0003, 0x0005, 0x0007, 0x0009, 0x000b, 0x000d, 0x000f
    };
    const size_t bitonic16 :: rhs[0x0050] = {
 0x0002, 0x0004, 0x0006, 0x0008, 0x000a, 0x000c, 0x000e, 0x0010,
 0x0003, 0x0004, 0x0008, 0x0007, 0x000c, 0x000b, 0x000f, 0x0010,
 0x0002, 0x0004, 0x0008, 0x0006, 0x000c, 0x000a, 0x000e, 0x0010,
 0x0008, 0x0007, 0x0006, 0x0005, 0x000d, 0x000e, 0x000f, 0x0010,
 0x0008, 0x0007, 0x0003, 0x0004, 0x000c, 0x000b, 0x000f, 0x0010,
 0x0008, 0x0006, 0x0004, 0x0002, 0x000a, 0x000c, 0x000e, 0x0010,
 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f, 0x0010,
 0x0008, 0x0007, 0x0006, 0x0005, 0x000d, 0x000e, 0x000f, 0x0010,
 0x0004, 0x0003, 0x0008, 0x0007, 0x000b, 0x000c, 0x000f, 0x0010,
 0x0002, 0x0004, 0x0006, 0x0008, 0x000a, 0x000c, 0x000e, 0x0010
    };
  }
}
