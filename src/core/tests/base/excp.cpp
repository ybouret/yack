

#include "yack/system/exception.hpp"
#include "yack/utest/run.hpp"
#include <cstring>

using namespace yack;

YACK_UTEST(excp)
{
    exception a = "Hello";
    a.show(std::cerr);

    exception b("in %s",__FILE__);
    b.show(std::cerr);

    exception c = a;
    c.show(std::cerr);

    a <<  ' ' << "World";
    a.show(std::cerr);
    a.add(" at %d mph",100);
    a.show(std::cerr);

    a.pre("From %g%%: ",42.0);
    a.show(std::cerr);

    libc::exception d(1,"bad in %s",__FILE__);
    d.show(std::cerr);

#if defined(YACK_WIN)
	win32::exception e(1, "bad in %s", __FILE__);
	e.show(std::cerr);
#endif
}
YACK_UDONE()

