#include "hibbard4.hpp"
namespace yack {
  namespace nwsrt {
    const char hibbard4 :: sid[] ="hibbard4";
    hibbard4 :: ~hibbard4() throw() {}
    hibbard4 ::  hibbard4() throw() : pairs<4,0x0005>(sid,lhs,rhs) {}
    const size_t hibbard4 :: lhs[0x0005] = {
 0x0001, 0x0003, 0x0001, 0x0002, 0x0002
    };
    const size_t hibbard4 :: rhs[0x0005] = {
 0x0002, 0x0004, 0x0003, 0x0004, 0x0003
    };
  }
}
