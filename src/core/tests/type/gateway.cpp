#include "yack/type/gateway.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace
{

    class dummy : public gateway<const int>
    {
    public:

        inline explicit dummy(const int a) noexcept : mine(a) {}
        inline virtual ~dummy() noexcept {}

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(dummy);
        int mine;
        virtual const_inward & bulk() const noexcept { return mine; }
    };

}

YACK_UTEST(type_gateway)
{
    dummy d(3);
    std::cerr << *d << std::endl;
}
YACK_UDONE()


