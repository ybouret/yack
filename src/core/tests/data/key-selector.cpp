#include "yack/associative/key-variety.hpp"
#include "yack/kr/digest.hpp"
#include "yack/utest/run.hpp"
#include <typeinfo>

using namespace yack;

namespace
{
    template <typename KEY>
    static inline void check_key(const key_variety::type guess,
                                 const char             *id)
    {
        static const key_variety::type kv = key_variety::cull<KEY>::variety;
        std::cerr << key_variety::text(kv) << " <== ";
        std::cerr << "KEY=<" << typeid(KEY).name() << "> = '" << id << "'";
        if(guess!=kv)
        {
            throw exception("key_variety mismatch");
        }
        std::cerr << " [OK]" << std::endl;

    }

    
    struct dummy
    {
    };
    
}

#define YACK_CHECK_KEY(GUESS,TYPE) \
check_key<TYPE>(key_variety::is_##GUESS,#TYPE);\
check_key<const TYPE>(key_variety::is_##GUESS,"const " #TYPE)

YACK_UTEST(data_key_selector)
{
    YACK_CHECK_KEY(integral_type,char);
    YACK_CHECK_KEY(integral_type,int32_t);
    YACK_CHECK_KEY(integral_type,void*);
    YACK_CHECK_KEY(not_available,dummy);
    YACK_CHECK_KEY(memory_buffer,digest);
    YACK_CHECK_KEY(legacy_string,char *);

#if 0
    YACK_CHECK_KEY(char);
    YACK_CHECK_KEY(int32_t);
    YACK_CHECK_KEY(dummy);
    YACK_CHECK_KEY(void*);
    YACK_CHECK_KEY(digest);
    YACK_CHECK_KEY(char *);
#endif
    
}
YACK_UDONE()


