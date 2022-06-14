#include "batcher29.hpp"
namespace yack {
  namespace nwsrt {
    const char batcher29 :: sid[] ="batcher29";
    batcher29 :: ~batcher29() throw() {}
    batcher29 ::  batcher29() throw() : swaps(sid,29,0x00ab,lhs,rhs) {}
    const size_t batcher29 :: lhs[0x00ab] = {
 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008,
 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x0001, 0x0002, 0x0003,
 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0011, 0x0012, 0x0013,
 0x0014, 0x0015, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e,
 0x000f, 0x0010, 0x0001, 0x0002, 0x0003, 0x0004, 0x0019, 0x0009,
 0x000a, 0x000b, 0x000c, 0x0011, 0x0012, 0x0013, 0x0014, 0x0001,
 0x0002, 0x0005, 0x0006, 0x0007, 0x0008, 0x000d, 0x000e, 0x000f,
 0x0010, 0x0001, 0x0005, 0x0006, 0x0007, 0x0008, 0x000d, 0x000e,
 0x000f, 0x0010, 0x0015, 0x0016, 0x0017, 0x0018, 0x0005, 0x0006,
 0x0009, 0x000a, 0x000d, 0x000e, 0x0011, 0x0012, 0x0015, 0x0016,
 0x0019, 0x001a, 0x0003, 0x0004, 0x0007, 0x0008, 0x000b, 0x000c,
 0x000f, 0x0003, 0x0004, 0x0007, 0x0008, 0x000b, 0x000c, 0x000f,
 0x0010, 0x0013, 0x0014, 0x0017, 0x0003, 0x0004, 0x0007, 0x0008,
 0x000b, 0x000c, 0x000f, 0x0010, 0x0013, 0x0014, 0x0017, 0x0018,
 0x001b, 0x0003, 0x0005, 0x0007, 0x0009, 0x000b, 0x000d, 0x000f,
 0x0011, 0x0013, 0x0015, 0x0017, 0x0019, 0x001b, 0x0002, 0x0004,
 0x0006, 0x0008, 0x000a, 0x000c, 0x000e, 0x0002, 0x0004, 0x0006,
 0x0008, 0x000a, 0x000c, 0x000e, 0x0010, 0x0012, 0x0014, 0x0016,
 0x0002, 0x0004, 0x0006, 0x0008, 0x000a, 0x000c, 0x000e, 0x0010,
 0x0012, 0x0014, 0x0016, 0x0018, 0x001a, 0x0002, 0x0004, 0x0006,
 0x0008, 0x000a, 0x000c, 0x000e, 0x0010, 0x0012, 0x0014, 0x0016,
 0x0018, 0x001a, 0x001c
    };
    const size_t batcher29 :: rhs[0x00ab] = {
 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018,
 0x0019, 0x001a, 0x001b, 0x001c, 0x001d, 0x0009, 0x000a, 0x000b,
 0x000c, 0x000d, 0x000e, 0x000f, 0x0010, 0x0019, 0x001a, 0x001b,
 0x001c, 0x001d, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016,
 0x0017, 0x0018, 0x0005, 0x0006, 0x0007, 0x0008, 0x001d, 0x000d,
 0x000e, 0x000f, 0x0010, 0x0015, 0x0016, 0x0017, 0x0018, 0x0003,
 0x0004, 0x0011, 0x0012, 0x0013, 0x0014, 0x0019, 0x001a, 0x001b,
 0x001c, 0x0002, 0x0009, 0x000a, 0x000b, 0x000c, 0x0011, 0x0012,
 0x0013, 0x0014, 0x0019, 0x001a, 0x001b, 0x001c, 0x0007, 0x0008,
 0x000b, 0x000c, 0x000f, 0x0010, 0x0013, 0x0014, 0x0017, 0x0018,
 0x001b, 0x001c, 0x0011, 0x0012, 0x0015, 0x0016, 0x0019, 0x001a,
 0x001d, 0x0009, 0x000a, 0x000d, 0x000e, 0x0011, 0x0012, 0x0015,
 0x0016, 0x0019, 0x001a, 0x001d, 0x0005, 0x0006, 0x0009, 0x000a,
 0x000d, 0x000e, 0x0011, 0x0012, 0x0015, 0x0016, 0x0019, 0x001a,
 0x001d, 0x0004, 0x0006, 0x0008, 0x000a, 0x000c, 0x000e, 0x0010,
 0x0012, 0x0014, 0x0016, 0x0018, 0x001a, 0x001c, 0x0011, 0x0013,
 0x0015, 0x0017, 0x0019, 0x001b, 0x001d, 0x0009, 0x000b, 0x000d,
 0x000f, 0x0011, 0x0013, 0x0015, 0x0017, 0x0019, 0x001b, 0x001d,
 0x0005, 0x0007, 0x0009, 0x000b, 0x000d, 0x000f, 0x0011, 0x0013,
 0x0015, 0x0017, 0x0019, 0x001b, 0x001d, 0x0003, 0x0005, 0x0007,
 0x0009, 0x000b, 0x000d, 0x000f, 0x0011, 0x0013, 0x0015, 0x0017,
 0x0019, 0x001b, 0x001d
    };
  }
}