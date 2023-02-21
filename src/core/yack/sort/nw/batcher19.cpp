#include "batcher19.hpp"
namespace yack {
  namespace nwsrt {
    const char batcher19 :: sid[] ="batcher19";
    batcher19 :: ~batcher19() noexcept {}
    batcher19 ::  batcher19() noexcept : swaps(sid,19,0x005b,lhs,rhs) {}
    const size_t batcher19 :: lhs[0x005b] = {
 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008,
 0x0001, 0x0002, 0x0003, 0x0004, 0x000c, 0x0009, 0x000a, 0x000b,
 0x0001, 0x0002, 0x0003, 0x0008, 0x0009, 0x000a, 0x000b, 0x0005,
 0x0006, 0x0007, 0x0001, 0x0002, 0x0005, 0x0006, 0x0007, 0x000d,
 0x000e, 0x000f, 0x0001, 0x0005, 0x0006, 0x0009, 0x000a, 0x000d,
 0x000e, 0x0011, 0x0004, 0x0003, 0x0004, 0x0007, 0x0008, 0x000c,
 0x0003, 0x000b, 0x0004, 0x0008, 0x000c, 0x0010, 0x0003, 0x0007,
 0x000b, 0x000f, 0x0003, 0x0005, 0x0007, 0x0009, 0x000b, 0x000d,
 0x000f, 0x0011, 0x0002, 0x0004, 0x0006, 0x0008, 0x0002, 0x0004,
 0x000a, 0x000c, 0x0002, 0x0004, 0x0006, 0x0008, 0x000a, 0x000c,
 0x000e, 0x0010, 0x0002, 0x0004, 0x0006, 0x0008, 0x000a, 0x000c,
 0x000e, 0x0010, 0x0012
    };
    const size_t batcher19 :: rhs[0x005b] = {
 0x0011, 0x0012, 0x0013, 0x000c, 0x000d, 0x000e, 0x000f, 0x0010,
 0x0009, 0x000a, 0x000b, 0x0008, 0x0010, 0x0011, 0x0012, 0x0013,
 0x0005, 0x0006, 0x0007, 0x000c, 0x000d, 0x000e, 0x000f, 0x0011,
 0x0012, 0x0013, 0x0003, 0x0004, 0x0009, 0x000a, 0x000b, 0x0011,
 0x0012, 0x0013, 0x0002, 0x0007, 0x0008, 0x000b, 0x000c, 0x000f,
 0x0010, 0x0013, 0x0012, 0x0011, 0x000a, 0x000d, 0x000e, 0x0012,
 0x0009, 0x0011, 0x0006, 0x000a, 0x000e, 0x0012, 0x0005, 0x0009,
 0x000d, 0x0011, 0x0004, 0x0006, 0x0008, 0x000a, 0x000c, 0x000e,
 0x0010, 0x0012, 0x0011, 0x0013, 0x000d, 0x000f, 0x0009, 0x000b,
 0x0011, 0x0013, 0x0005, 0x0007, 0x0009, 0x000b, 0x000d, 0x000f,
 0x0011, 0x0013, 0x0003, 0x0005, 0x0007, 0x0009, 0x000b, 0x000d,
 0x000f, 0x0011, 0x0013
    };
  }
}
