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
        static const key_variety::type kv = key_variety::cull<KEY>::value;
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

#define SHOW(EXPR) std::cerr << std::setw(40) << #EXPR << " : " << ( (EXPR) ? "true" : "false" ) << std::endl

YACK_UTEST(data_key_variety)
{

    
    
    SHOW(type_traits<char>::is_array);
    SHOW(type_traits<const char>::is_array);
    SHOW(type_traits<char[]>::is_array);
    SHOW(type_traits<const char[]>::is_array);
    SHOW(type_traits<char [2]>::is_array);
    SHOW(type_traits<const char [2]>::is_array);


    YACK_CHECK_KEY(integral_type,char);
    YACK_CHECK_KEY(integral_type,int32_t);
    YACK_CHECK_KEY(integral_type,void*);
    YACK_CHECK_KEY(not_available,dummy);
    YACK_CHECK_KEY(memory_buffer,digest);
    YACK_CHECK_KEY(legacy_string,char *);
    YACK_CHECK_KEY(legacy_string,char []);
    YACK_CHECK_KEY(legacy_string,char [1]);
    YACK_CHECK_KEY(legacy_string,char [2]);
    YACK_CHECK_KEY(legacy_string,char [4]);

}
YACK_UDONE()


