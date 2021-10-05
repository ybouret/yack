#include "yack/singleton.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace
{
    class dummy : public singleton<dummy>
    {
    public:
        static const at_exit::longevity life_time = 10;
        static const size_t guess=1;
        const size_t value;
        
        virtual ~dummy() throw()
        {
            std::cerr << "-Dummy@" << value << std::endl;
        }
        
        inline static void suppress() throw() { suppress_(); };
        
    private:
        friend class singleton<dummy>;
        
        explicit dummy() throw() : value(guess)
        {
            std::cerr << "+Dummy@" << value << std::endl;
        }
        
       
    };
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


