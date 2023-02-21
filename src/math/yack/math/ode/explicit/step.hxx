namespace yack
{
    namespace math
    {
        namespace ode
        {
            template <> explicit_step<real_t>:: ~explicit_step() noexcept {}
            template <> explicit_step<real_t>::  explicit_step(const size_t num_arrays) : step<real_t>(num_arrays) {}
            
        }
    }

}
