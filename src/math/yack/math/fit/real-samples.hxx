

namespace yack
{
    namespace math
    {

        namespace fit
        {

            template <>
            real_samples_of<real_t>:: ~real_samples_of() throw()
            {
            }

            template <>
            real_sample_of<real_t> & real_samples_of<real_t>:: add(real_sample_type *who)
            {
                assert(who);
                const shared_type ptr = who;
                use(ptr);
                return *who;
            }


        }

    }

}


