#include "batcher28.hpp"
namespace yack {
  namespace nwsrt {
    const char batcher28 :: sid[] ="batcher28";
    batcher28 :: ~batcher28() throw() {}
    batcher28 ::  batcher28() throw() : pairs<28,0x00a1>(sid,lhs,rhs) {}
    const size_t batcher28 :: lhs[0x00a1] = {
 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008,
 0x0009, 0x000a, 0x000b, 0x000c, 0x0001, 0x0002, 0x0003, 0x0004,
 0x0005, 0x0006, 0x0007, 0x0008, 0x0011, 0x0012, 0x0013, 0x0014,
 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f, 0x0010,
 0x0001, 0x0002, 0x0003, 0x0004, 0x0009, 0x000a, 0x000b, 0x000c,
 0x0011, 0x0012, 0x0013, 0x0014, 0x0001, 0x0002, 0x0005, 0x0006,
 0x0007, 0x0008, 0x000d, 0x000e, 0x000f, 0x0010, 0x0001, 0x0005,
 0x0006, 0x0007, 0x0008, 0x000d, 0x000e, 0x000f, 0x0010, 0x0015,
 0x0016, 0x0017, 0x0018, 0x0005, 0x0006, 0x0009, 0x000a, 0x000d,
 0x000e, 0x0011, 0x0012, 0x0015, 0x0016, 0x0019, 0x001a, 0x0003,
 0x0004, 0x0007, 0x0008, 0x000b, 0x000c, 0x001b, 0x0003, 0x0004,
 0x0007, 0x0008, 0x000b, 0x000c, 0x000f, 0x0010, 0x0013, 0x0014,
 0x0003, 0x0004, 0x0007, 0x0008, 0x000b, 0x000c, 0x000f, 0x0010,
 0x0013, 0x0014, 0x0017, 0x0018, 0x0003, 0x0005, 0x0007, 0x0009,
 0x000b, 0x000d, 0x000f, 0x0011, 0x0013, 0x0015, 0x0017, 0x0019,
 0x0002, 0x0004, 0x0006, 0x0008, 0x000a, 0x000c, 0x0002, 0x0004,
 0x0006, 0x0008, 0x000a, 0x000c, 0x000e, 0x0010, 0x0012, 0x0014,
 0x0002, 0x0004, 0x0006, 0x0008, 0x000a, 0x000c, 0x000e, 0x0010,
 0x0012, 0x0014, 0x0016, 0x0018, 0x0002, 0x0004, 0x0006, 0x0008,
 0x000a, 0x000c, 0x000e, 0x0010, 0x0012, 0x0014, 0x0016, 0x0018,
 0x001a
    };
    const size_t batcher28 :: rhs[0x00a1] = {
 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018,
 0x0019, 0x001a, 0x001b, 0x001c, 0x0009, 0x000a, 0x000b, 0x000c,
 0x000d, 0x000e, 0x000f, 0x0010, 0x0019, 0x001a, 0x001b, 0x001c,
 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018,
 0x0005, 0x0006, 0x0007, 0x0008, 0x000d, 0x000e, 0x000f, 0x0010,
 0x0015, 0x0016, 0x0017, 0x0018, 0x0003, 0x0004, 0x0011, 0x0012,
 0x0013, 0x0014, 0x0019, 0x001a, 0x001b, 0x001c, 0x0002, 0x0009,
 0x000a, 0x000b, 0x000c, 0x0011, 0x0012, 0x0013, 0x0014, 0x0019,
 0x001a, 0x001b, 0x001c, 0x0007, 0x0008, 0x000b, 0x000c, 0x000f,
 0x0010, 0x0013, 0x0014, 0x0017, 0x0018, 0x001b, 0x001c, 0x0011,
 0x0012, 0x0015, 0x0016, 0x0019, 0x001a, 0x001c, 0x0009, 0x000a,
 0x000d, 0x000e, 0x0011, 0x0012, 0x0015, 0x0016, 0x0019, 0x001a,
 0x0005, 0x0006, 0x0009, 0x000a, 0x000d, 0x000e, 0x0011, 0x0012,
 0x0015, 0x0016, 0x0019, 0x001a, 0x0004, 0x0006, 0x0008, 0x000a,
 0x000c, 0x000e, 0x0010, 0x0012, 0x0014, 0x0016, 0x0018, 0x001a,
 0x0011, 0x0013, 0x0015, 0x0017, 0x0019, 0x001b, 0x0009, 0x000b,
 0x000d, 0x000f, 0x0011, 0x0013, 0x0015, 0x0017, 0x0019, 0x001b,
 0x0005, 0x0007, 0x0009, 0x000b, 0x000d, 0x000f, 0x0011, 0x0013,
 0x0015, 0x0017, 0x0019, 0x001b, 0x0003, 0x0005, 0x0007, 0x0009,
 0x000b, 0x000d, 0x000f, 0x0011, 0x0013, 0x0015, 0x0017, 0x0019,
 0x001b
    };
  }
}
