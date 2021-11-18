#include "yack/associative/key-variety.hpp"
#include "yack/kr/digest.hpp"
#include "yack/utest/run.hpp"
#include <typeinfo>

using namespace yack;

namespace
{
    template <typename KEY>
    static inline void check_key(const char *id)
    {
        static const key_variety::type kv = key_variety::cull<KEY>::variety;
        std::cerr << key_variety::text(kv) << " <== ";
        std::cerr << "KEY=<" << typeid(KEY).name() << "> = '" << id << "'";
        std::cerr << std::endl;
    }

    
    struct dummy
    {
    };
    
}

#define YACK_CHECK_KEY(TYPE) check_key<TYPE>(#TYPE); check_key<const TYPE>("const " #TYPE)

YACK_UTEST(data_key_selector)
{
    YACK_CHECK_KEY(char);
    YACK_CHECK_KEY(int32_t);
    YACK_CHECK_KEY(dummy);
    YACK_CHECK_KEY(void*);
    YACK_CHECK_KEY(digest);
    YACK_CHECK_KEY(char *);

}
YACK_UDONE()


