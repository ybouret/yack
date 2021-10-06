#include "yack/singleton.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace
{
    class dummy : public singleton<dummy>
    {
    public:
        static const at_exit::longevity life_time   = 10;
        static const char               call_sign[];
        static const size_t             guess=13
        ;
        const size_t value;
        
        virtual ~dummy() throw()
        {
            std::cerr << "-" << call_sign << "@" << value << std::endl;
        }
        
        inline static void suppress() throw() { suppress_(); };
        
    private:
        friend class singleton<dummy>;
        
        explicit dummy() throw() : value(guess)
        {
            std::cerr << "+" << call_sign << "@" << value << std::endl;
        }

    };
    const char dummy::call_sign[] = "dummy";

}

YACK_UTEST(singleton)
{
    size_t guess = dummy::instance().value;
    YACK_CHECK(dummy::guess==guess);
    YACK_CHECK(dummy::exists());
    dummy::suppress();
    YACK_CHECK(!dummy::exists());
    (void) dummy::instance();
    YACK_CHECK(dummy::exists());
}
YACK_UDONE()


