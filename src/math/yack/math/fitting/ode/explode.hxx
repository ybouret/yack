
namespace yack
{
    namespace math
    {

        namespace fitting
        {

            template <> explODE<real_t>:: ~explODE() throw() {}



            template <> explODE<real_t>:: explODE(const app_ptr &usr) :
            seq_type(),
            app( usr ),
            Y()
            {
            }

            
        }
    }
}

