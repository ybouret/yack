
#include "yack/arith/prime-pi.hpp"

namespace yack
{

    namespace core
    {
        const uint64_t prime_pi::table[] =
        {
            0, 1, 2, 4, 6, 11, 18, 31, 54, 97, 172, 309, 564, 1028, 1900, 3512,
            6542, 12251, 23000, 43390, 82025, 155611, 295947, 564163, 1077871,
            2063689, 3957809, 7603553, 14630843, 28192750, 54400028, 105097565,
            203280221, 393615806, 762939111, 1480206279, 2874398515, 5586502348,
            10866266172, 21151907950, 41203088796, 80316571436, 156661034233,
            305761713237, 597116381732, 1166746786182, 2280998753949,
            4461632979717, 8731188863470, 17094432576778,
            33483379603407, 65612899915304, 128625503610475
        };

        const size_t prime_pi::count = sizeof(table)/sizeof(table[0]);
    }
}
