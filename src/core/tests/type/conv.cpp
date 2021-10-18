
#include "yack/type/conversion.hpp"
#include "yack/concurrent/fake-lock.hpp"
#include "yack/object.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(type_conv)
{
    YACK_CHECK(YACK_IS_SUPERSUBCLASS(lockable,fake_lock));
    YACK_CHECK(YACK_IS_SUPERSUBCLASS(fake_lock,fake_lock));
    YACK_CHECK(YACK_IS_SUPERSUBCLASS_STRICT(lockable,fake_lock));
    YACK_CHECK(!YACK_IS_SUPERSUBCLASS_STRICT(fake_lock,fake_lock));
    YACK_CHECK(!YACK_IS_SUPERSUBCLASS(object,fake_lock));

}
YACK_UDONE()

