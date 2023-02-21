
namespace yack
{
    namespace math
    {

        namespace ode
        {
            template <> zextr<real_t>:: ~zextr() noexcept
            {
            }

            template <> zextr<real_t>:: zextr(const size_t num_arrays) :
            object(), counted(), tableaux(num_arrays,0)
            {
            }
        }

    }
}

