
namespace yack
{
    namespace math
    {

        namespace fit
        {

            template <>
            real_sample_of<real_t>:: ~real_sample_of() throw()
            {
            }

            template<>
            int real_sample_of<real_t>:: compare(const real_t &lhs, const real_t &rhs) throw()
            {
                return lhs<rhs ? 1 : rhs<lhs ? -1 : 0;
            }
            
        }

    }

}


