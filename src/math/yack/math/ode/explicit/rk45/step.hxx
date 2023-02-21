namespace yack
{
    namespace math
    {
        namespace ode
        {

            template <> rk45_step<real_t>:: ~rk45_step() noexcept {}
            template <> rk45_step<real_t>::  rk45_step(const size_t n) : arrays_of<real_t>(n,0)
            {
            }

        }
    }
}
