#include "batcher32.hpp"
namespace yack {
  namespace nwsrt {
    const char batcher32 :: sid[] ="batcher32";
    batcher32 :: ~batcher32() throw() {}
    batcher32 ::  batcher32() throw() : swaps(sid,32,0x00bf,lhs,rhs) {}
    const size_t batcher32 :: lhs[0x00bf] = {
 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008,
 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f, 0x0010,
 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008,
 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018,
 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f, 0x0010,
 0x0001, 0x0002, 0x0003, 0x0004, 0x0019, 0x001a, 0x001b, 0x001c,
 0x0009, 0x000a, 0x000b, 0x000c, 0x0011, 0x0012, 0x0013, 0x0014,
 0x0001, 0x0002, 0x001d, 0x001e, 0x0005, 0x0006, 0x0007, 0x0008,
 0x000d, 0x000e, 0x000f, 0x0010, 0x0001, 0x001f, 0x0005, 0x0006,
 0x0007, 0x0008, 0x000d, 0x000e, 0x000f, 0x0010, 0x0015, 0x0016,
 0x0017, 0x0018, 0x0005, 0x0006, 0x0009, 0x000a, 0x000d, 0x000e,
 0x0011, 0x0012, 0x0015, 0x0016, 0x0019, 0x001a, 0x0003, 0x0004,
 0x0007, 0x0008, 0x000b, 0x000c, 0x000f, 0x0010, 0x0003, 0x0004,
 0x0007, 0x0008, 0x000b, 0x000c, 0x000f, 0x0010, 0x0013, 0x0014,
 0x0017, 0x0018, 0x0003, 0x0004, 0x0007, 0x0008, 0x000b, 0x000c,
 0x000f, 0x0010, 0x0013, 0x0014, 0x0017, 0x0018, 0x001b, 0x001c,
 0x0003, 0x0005, 0x0007, 0x0009, 0x000b, 0x000d, 0x000f, 0x0011,
 0x0013, 0x0015, 0x0017, 0x0019, 0x001b, 0x001d, 0x0002, 0x0004,
 0x0006, 0x0008, 0x000a, 0x000c, 0x000e, 0x0010, 0x0002, 0x0004,
 0x0006, 0x0008, 0x000a, 0x000c, 0x000e, 0x0010, 0x0012, 0x0014,
 0x0016, 0x0018, 0x0002, 0x0004, 0x0006, 0x0008, 0x000a, 0x000c,
 0x000e, 0x0010, 0x0012, 0x0014, 0x0016, 0x0018, 0x001a, 0x001c,
 0x0002, 0x0004, 0x0006, 0x0008, 0x000a, 0x000c, 0x000e, 0x0010,
 0x0012, 0x0014, 0x0016, 0x0018, 0x001a, 0x001c, 0x001e
    };
    const size_t batcher32 :: rhs[0x00bf] = {
 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018,
 0x0019, 0x001a, 0x001b, 0x001c, 0x001d, 0x001e, 0x001f, 0x0020,
 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f, 0x0010,
 0x0019, 0x001a, 0x001b, 0x001c, 0x001d, 0x001e, 0x001f, 0x0020,
 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018,
 0x0005, 0x0006, 0x0007, 0x0008, 0x001d, 0x001e, 0x001f, 0x0020,
 0x000d, 0x000e, 0x000f, 0x0010, 0x0015, 0x0016, 0x0017, 0x0018,
 0x0003, 0x0004, 0x001f, 0x0020, 0x0011, 0x0012, 0x0013, 0x0014,
 0x0019, 0x001a, 0x001b, 0x001c, 0x0002, 0x0020, 0x0009, 0x000a,
 0x000b, 0x000c, 0x0011, 0x0012, 0x0013, 0x0014, 0x0019, 0x001a,
 0x001b, 0x001c, 0x0007, 0x0008, 0x000b, 0x000c, 0x000f, 0x0010,
 0x0013, 0x0014, 0x0017, 0x0018, 0x001b, 0x001c, 0x0011, 0x0012,
 0x0015, 0x0016, 0x0019, 0x001a, 0x001d, 0x001e, 0x0009, 0x000a,
 0x000d, 0x000e, 0x0011, 0x0012, 0x0015, 0x0016, 0x0019, 0x001a,
 0x001d, 0x001e, 0x0005, 0x0006, 0x0009, 0x000a, 0x000d, 0x000e,
 0x0011, 0x0012, 0x0015, 0x0016, 0x0019, 0x001a, 0x001d, 0x001e,
 0x0004, 0x0006, 0x0008, 0x000a, 0x000c, 0x000e, 0x0010, 0x0012,
 0x0014, 0x0016, 0x0018, 0x001a, 0x001c, 0x001e, 0x0011, 0x0013,
 0x0015, 0x0017, 0x0019, 0x001b, 0x001d, 0x001f, 0x0009, 0x000b,
 0x000d, 0x000f, 0x0011, 0x0013, 0x0015, 0x0017, 0x0019, 0x001b,
 0x001d, 0x001f, 0x0005, 0x0007, 0x0009, 0x000b, 0x000d, 0x000f,
 0x0011, 0x0013, 0x0015, 0x0017, 0x0019, 0x001b, 0x001d, 0x001f,
 0x0003, 0x0005, 0x0007, 0x0009, 0x000b, 0x000d, 0x000f, 0x0011,
 0x0013, 0x0015, 0x0017, 0x0019, 0x001b, 0x001d, 0x001f
    };
  }
}
