
namespace yack
{
    namespace math
    {

        namespace fitting
        {

            template <> gls<real_t>:: ~gls() throw()
            {
            }

            template <> gls<real_t>:: gls(const drvs_ptr *ppDrvs) :
            ls_type(ppDrvs)
            {
            }


        }

    }

}

