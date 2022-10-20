
namespace yack
{
    namespace math
    {

        namespace fitting
        {

            template <> explODE<real_t>:: ~explODE() throw() {}



            template <> explODE<real_t>:: explODE(const app_ptr &usr,
                                                  const size_t   dim) :
            seq_type(),
            arr_type(dim),
            app( usr )
            {
            }

            
            
        }
    }
}

