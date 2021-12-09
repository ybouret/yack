#include "yack/jive/pattern/posix.hpp"
#include "yack/jive/tags.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

#define YPOSIX(NAME) \
do { \
/**/ const auto_ptr<jive::pattern> p = jive::posix::NAME();\
/**/ p->makeGV( #NAME ".dot" ); \
/**/ const auto_ptr<jive::pattern> q = jive::posix::create(#NAME);\
/**/ YACK_CHECK( *q == *p );\
} \
while(false)

YACK_UTEST(posix)
{
    YPOSIX(lower);
    YPOSIX(upper);
    YPOSIX(alpha);
    YPOSIX(digit);
    YPOSIX(alnum);
    YPOSIX(xdigit);
    YPOSIX(blank);
    YPOSIX(space);
    YPOSIX(punct);

    YPOSIX(word);
    YPOSIX(endl);
    YPOSIX(dot);
    YPOSIX(core);
    YPOSIX(vowel);
    YPOSIX(consonant);
    
    vector<string> keys;
    jive::posix::collect(keys);
    std::cerr << "keys=" << keys << std::endl;
}
YACK_UDONE()


