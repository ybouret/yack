
namespace yack
{
    namespace math
    {
        namespace ode
        {
            template <> dormand_prince<real_t>:: ~dormand_prince() throw() {}

            template <> dormand_prince<real_t>::  dormand_prince() :
            base_type(6)
#if 0
            ,ak2( next() ),
            ak3( next() ),
            ak4( next() ),
            ak5( next() ),
            ak6( next() ),
            ytemp( next() )
#endif
            {

            }

        }
    }
}

