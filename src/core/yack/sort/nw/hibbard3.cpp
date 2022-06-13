#include "hibbard3.hpp"
namespace yack {
  namespace nwsrt {
    const char hibbard3 :: sid[] ="hibbard3";
    hibbard3 :: ~hibbard3() throw() {}
    hibbard3 ::  hibbard3() throw() : pairs<3,0x0003>(sid,lhs,rhs) {}
    const size_t hibbard3 :: lhs[0x0003] = {
 0x0001, 0x0001, 0x0002
    };
    const size_t hibbard3 :: rhs[0x0003] = {
 0x0002, 0x0003, 0x0003
    };
  }
}
