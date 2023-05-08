#include "yack/math/geometry/pcontour.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace  math
    {
        namespace kernel
        {
            static const char fn[] = "periodic_contour";

            periodic_contour:: periodic_contour(const size_t n)
            {
                if(n<3) throw imported::exception(fn,"no enough points");
            }

            periodic_contour:: ~periodic_contour() noexcept
            {
            }

            void    periodic_contour:: raise_singular() const
            {
                throw imported::exception(fn,"singular vertices");
            }

        }
    }
}
