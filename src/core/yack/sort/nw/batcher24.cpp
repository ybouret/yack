#include "batcher24.hpp"
namespace yack {
  namespace nwsrt {
    const char batcher24 :: sid[] ="batcher24";
    batcher24 :: ~batcher24() noexcept {}
    batcher24 ::  batcher24() noexcept : swaps(sid,24,0x007f,lhs,rhs) {}
    const size_t batcher24 :: lhs[0x007f] = {
 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008,
 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008,
 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f, 0x0010,
 0x0001, 0x0002, 0x0003, 0x0004, 0x0009, 0x000a, 0x000b, 0x000c,
 0x0011, 0x0012, 0x0013, 0x0014, 0x0001, 0x0002, 0x0005, 0x0006,
 0x0007, 0x0008, 0x0015, 0x0016, 0x0001, 0x0005, 0x0006, 0x0007,
 0x0008, 0x000d, 0x000e, 0x000f, 0x0010, 0x0017, 0x0005, 0x0006,
 0x0009, 0x000a, 0x000d, 0x000e, 0x0011, 0x0012, 0x0003, 0x0004,
 0x0007, 0x0008, 0x0003, 0x0004, 0x0007, 0x0008, 0x000b, 0x000c,
 0x000f, 0x0010, 0x0003, 0x0004, 0x0007, 0x0008, 0x000b, 0x000c,
 0x000f, 0x0010, 0x0013, 0x0014, 0x0003, 0x0005, 0x0007, 0x0009,
 0x000b, 0x000d, 0x000f, 0x0011, 0x0013, 0x0015, 0x0002, 0x0004,
 0x0006, 0x0008, 0x0002, 0x0004, 0x0006, 0x0008, 0x000a, 0x000c,
 0x000e, 0x0010, 0x0002, 0x0004, 0x0006, 0x0008, 0x000a, 0x000c,
 0x000e, 0x0010, 0x0012, 0x0014, 0x0002, 0x0004, 0x0006, 0x0008,
 0x000a, 0x000c, 0x000e, 0x0010, 0x0012, 0x0014, 0x0016
    };
    const size_t batcher24 :: rhs[0x007f] = {
 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018,
 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f, 0x0010,
 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018,
 0x0005, 0x0006, 0x0007, 0x0008, 0x000d, 0x000e, 0x000f, 0x0010,
 0x0015, 0x0016, 0x0017, 0x0018, 0x0003, 0x0004, 0x0011, 0x0012,
 0x0013, 0x0014, 0x0017, 0x0018, 0x0002, 0x0009, 0x000a, 0x000b,
 0x000c, 0x0011, 0x0012, 0x0013, 0x0014, 0x0018, 0x0007, 0x0008,
 0x000b, 0x000c, 0x000f, 0x0010, 0x0013, 0x0014, 0x0011, 0x0012,
 0x0015, 0x0016, 0x0009, 0x000a, 0x000d, 0x000e, 0x0011, 0x0012,
 0x0015, 0x0016, 0x0005, 0x0006, 0x0009, 0x000a, 0x000d, 0x000e,
 0x0011, 0x0012, 0x0015, 0x0016, 0x0004, 0x0006, 0x0008, 0x000a,
 0x000c, 0x000e, 0x0010, 0x0012, 0x0014, 0x0016, 0x0011, 0x0013,
 0x0015, 0x0017, 0x0009, 0x000b, 0x000d, 0x000f, 0x0011, 0x0013,
 0x0015, 0x0017, 0x0005, 0x0007, 0x0009, 0x000b, 0x000d, 0x000f,
 0x0011, 0x0013, 0x0015, 0x0017, 0x0003, 0x0005, 0x0007, 0x0009,
 0x000b, 0x000d, 0x000f, 0x0011, 0x0013, 0x0015, 0x0017
    };
  }
}
