namespace yack
{
    namespace math
    {
        namespace ode
        {
            template <> step<real_t>:: ~step() noexcept {}
            template <> step<real_t>::  step(const size_t num_arrays) :
            entity(),
            tableaux(num_arrays,0) {}
            
        }
    }

}
