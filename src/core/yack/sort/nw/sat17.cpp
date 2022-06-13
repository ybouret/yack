#include "sat17.hpp"
namespace yack {
  namespace nwsrt {
    const char sat17 :: sid[] ="sat17";
    sat17 :: ~sat17() throw() {}
    sat17 ::  sat17() throw() : pairs<17,0x004f>(sid,lhs,rhs) {}
    const size_t sat17 :: lhs[0x004f] = {
 0x0002, 0x0004, 0x0006, 0x0008, 0x000a, 0x000c, 0x000e, 0x0010,
 0x0003, 0x0007, 0x000b, 0x000f, 0x0002, 0x0006, 0x000a, 0x000e,
 0x0005, 0x000d, 0x0004, 0x000c, 0x0003, 0x000b, 0x0002, 0x000a,
 0x0001, 0x0005, 0x0009, 0x0002, 0x000f, 0x0007, 0x0006, 0x0003,
 0x0002, 0x0004, 0x0008, 0x0005, 0x0001, 0x0003, 0x0009, 0x000b,
 0x0001, 0x0003, 0x000a, 0x0004, 0x0008, 0x000d, 0x0007, 0x0006,
 0x0003, 0x0005, 0x000c, 0x0004, 0x000a, 0x0002, 0x0007, 0x0002,
 0x0005, 0x0008, 0x000b, 0x000e, 0x0001, 0x0006, 0x000d, 0x0001,
 0x0003, 0x0005, 0x0007, 0x000a, 0x000d, 0x000f, 0x0008, 0x0002,
 0x0004, 0x0006, 0x0008, 0x000a, 0x000c, 0x000e, 0x0010
    };
    const size_t sat17 :: rhs[0x004f] = {
 0x0003, 0x0005, 0x0007, 0x0009, 0x000b, 0x000d, 0x000f, 0x0011,
 0x0005, 0x0009, 0x000d, 0x0011, 0x0004, 0x0008, 0x000c, 0x0010,
 0x0009, 0x0011, 0x0008, 0x0010, 0x0007, 0x000f, 0x0006, 0x000e,
 0x0004, 0x0008, 0x0011, 0x000e, 0x0010, 0x000d, 0x000c, 0x000b,
 0x0011, 0x0007, 0x0010, 0x000f, 0x000e, 0x0006, 0x000a, 0x000c,
 0x0002, 0x0009, 0x0010, 0x0005, 0x000c, 0x000f, 0x000e, 0x000b,
 0x0010, 0x000b, 0x000e, 0x0009, 0x000d, 0x0006, 0x0008, 0x0004,
 0x0007, 0x000a, 0x000c, 0x0010, 0x0003, 0x0009, 0x000f, 0x0002,
 0x0004, 0x0006, 0x0009, 0x000c, 0x000e, 0x0010, 0x000b, 0x0003,
 0x0005, 0x0007, 0x0009, 0x000b, 0x000d, 0x000f, 0x0011
    };
  }
}
