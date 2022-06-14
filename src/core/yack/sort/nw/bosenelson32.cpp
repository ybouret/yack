#include "bosenelson32.hpp"
namespace yack {
  namespace nwsrt {
    const char bosenelson32 :: sid[] ="bosenelson32";
    bosenelson32 :: ~bosenelson32() throw() {}
    bosenelson32 ::  bosenelson32() throw() : swaps(sid,32,0x00d3,lhs,rhs) {}
    const size_t bosenelson32 :: lhs[0x00d3] = {
 0x0001, 0x0003, 0x0005, 0x0007, 0x0009, 0x000b, 0x000d, 0x000f,
 0x0011, 0x0013, 0x0015, 0x0017, 0x0019, 0x001b, 0x001d, 0x001f,
 0x0001, 0x0002, 0x0005, 0x0006, 0x0009, 0x000a, 0x000d, 0x000e,
 0x0011, 0x0012, 0x0015, 0x0016, 0x0019, 0x001a, 0x001d, 0x001e,
 0x0002, 0x0006, 0x0001, 0x0004, 0x000a, 0x000e, 0x0009, 0x000c,
 0x0012, 0x0016, 0x0011, 0x0014, 0x001a, 0x001e, 0x0019, 0x001c,
 0x0002, 0x0003, 0x000a, 0x000b, 0x0001, 0x0008, 0x0012, 0x0013,
 0x001a, 0x001b, 0x0011, 0x0018, 0x0002, 0x0004, 0x000a, 0x000c,
 0x0012, 0x0014, 0x001a, 0x001c, 0x0001, 0x0010, 0x0003, 0x0004,
 0x000b, 0x000c, 0x0002, 0x0007, 0x0013, 0x0014, 0x001b, 0x001c,
 0x0012, 0x0017, 0x0004, 0x000c, 0x0002, 0x0003, 0x0006, 0x0008,
 0x0014, 0x001c, 0x0012, 0x0013, 0x0016, 0x0018, 0x0004, 0x0003,
 0x0005, 0x0008, 0x0014, 0x0013, 0x0015, 0x0018, 0x0002, 0x000f,
 0x0004, 0x0006, 0x0014, 0x0016, 0x0002, 0x0003, 0x000e, 0x0010,
 0x0004, 0x0007, 0x0014, 0x0017, 0x0003, 0x0010, 0x0004, 0x0008,
 0x0006, 0x0007, 0x0014, 0x0018, 0x0016, 0x0017, 0x0005, 0x0008,
 0x0015, 0x0018, 0x0004, 0x000d, 0x0006, 0x0008, 0x0016, 0x0018,
 0x0004, 0x0005, 0x000c, 0x000e, 0x0007, 0x0008, 0x0017, 0x0018,
 0x0004, 0x0006, 0x000b, 0x000f, 0x0008, 0x0018, 0x0004, 0x0006,
 0x0007, 0x000a, 0x000c, 0x0010, 0x0008, 0x0007, 0x0005, 0x0006,
 0x0009, 0x000c, 0x000f, 0x0010, 0x0008, 0x0006, 0x0007, 0x000a,
 0x000e, 0x0010, 0x0008, 0x0007, 0x000b, 0x0010, 0x0008, 0x000c,
 0x0008, 0x000d, 0x0008, 0x000e, 0x000c, 0x000d, 0x0008, 0x000f,
 0x000b, 0x000e, 0x0008, 0x0010, 0x000a, 0x000c, 0x000e, 0x000f,
 0x0009, 0x000c, 0x0010, 0x000f, 0x000a, 0x0010, 0x000e, 0x000f,
 0x000b, 0x0010, 0x000c, 0x0010, 0x000d, 0x0010, 0x000e, 0x0010,
 0x000f, 0x0010, 0x0010
    };
    const size_t bosenelson32 :: rhs[0x00d3] = {
 0x0002, 0x0004, 0x0006, 0x0008, 0x000a, 0x000c, 0x000e, 0x0010,
 0x0012, 0x0014, 0x0016, 0x0018, 0x001a, 0x001c, 0x001e, 0x0020,
 0x0003, 0x0004, 0x0007, 0x0008, 0x000b, 0x000c, 0x000f, 0x0010,
 0x0013, 0x0014, 0x0017, 0x0018, 0x001b, 0x001c, 0x001f, 0x0020,
 0x0003, 0x0007, 0x0005, 0x0008, 0x000b, 0x000f, 0x000d, 0x0010,
 0x0013, 0x0017, 0x0015, 0x0018, 0x001b, 0x001f, 0x001d, 0x0020,
 0x0006, 0x0007, 0x000e, 0x000f, 0x0009, 0x0010, 0x0016, 0x0017,
 0x001e, 0x001f, 0x0019, 0x0020, 0x0005, 0x0007, 0x000d, 0x000f,
 0x0015, 0x0017, 0x001d, 0x001f, 0x0011, 0x0020, 0x0005, 0x0006,
 0x000d, 0x000e, 0x000a, 0x000f, 0x0015, 0x0016, 0x001d, 0x001e,
 0x001a, 0x001f, 0x0005, 0x000d, 0x0009, 0x000b, 0x000e, 0x000f,
 0x0015, 0x001d, 0x0019, 0x001b, 0x001e, 0x001f, 0x000c, 0x0009,
 0x000d, 0x000e, 0x001c, 0x0019, 0x001d, 0x001e, 0x0012, 0x001f,
 0x000b, 0x000d, 0x001b, 0x001d, 0x0011, 0x0013, 0x001e, 0x001f,
 0x000a, 0x000d, 0x001a, 0x001d, 0x0011, 0x001e, 0x0009, 0x000d,
 0x000a, 0x000b, 0x0019, 0x001d, 0x001a, 0x001b, 0x0009, 0x000c,
 0x0019, 0x001c, 0x0014, 0x001d, 0x0009, 0x000b, 0x0019, 0x001b,
 0x0013, 0x0015, 0x001c, 0x001d, 0x0009, 0x000a, 0x0019, 0x001a,
 0x0012, 0x0016, 0x001b, 0x001d, 0x0009, 0x0019, 0x0011, 0x0015,
 0x0017, 0x001a, 0x001b, 0x001d, 0x0018, 0x0015, 0x0011, 0x0012,
 0x0019, 0x001a, 0x001b, 0x001c, 0x0017, 0x0011, 0x0013, 0x0019,
 0x001a, 0x001b, 0x0016, 0x0011, 0x0019, 0x001a, 0x0015, 0x0019,
 0x0014, 0x0019, 0x0013, 0x0019, 0x0014, 0x0015, 0x0012, 0x0019,
 0x0013, 0x0016, 0x0011, 0x0019, 0x0012, 0x0013, 0x0015, 0x0017,
 0x0011, 0x0012, 0x0018, 0x0015, 0x0011, 0x0017, 0x0012, 0x0013,
 0x0011, 0x0016, 0x0011, 0x0015, 0x0011, 0x0014, 0x0011, 0x0013,
 0x0011, 0x0012, 0x0011
    };
  }
}