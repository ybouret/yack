#include "yack/type/args.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

static inline const char *answer(const bool flag) throw() { return flag ? "yes" : "no"; }

#define CHECK(ppty) do { std::cerr << "\t" << std::setw(20) << #ppty << " : " << answer( type_traits<T>::is_##ppty ) << std::endl; } while(false)

template <typename T>
static inline void display_traits(const char *name)
{
    std::cerr << "<" << name << ">" << std::endl;
    CHECK(const);
    CHECK(volatile);
    CHECK(qualified);
    CHECK(pointer);
    CHECK(pointer_to_member);
    CHECK(reference);
    CHECK(std_integer);
    CHECK(std_unsigned);
    CHECK(sys_integer);
    CHECK(sys_unsigned);
    CHECK(integral);
    CHECK(std_real);
    CHECK(arithmetic);
    CHECK(fundamental);
    CHECK(primitive);

}

#define DISPLAY_TRAITS(TYPE) display_traits<TYPE>( #TYPE )

YACK_UTEST(type_traits)
{
    DISPLAY_TRAITS(int);
    DISPLAY_TRAITS(const int);
    DISPLAY_TRAITS(int &);
    DISPLAY_TRAITS(const int &);
}
YACK_UDONE()

