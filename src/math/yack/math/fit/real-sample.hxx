
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

            template <>
            void  real_sample_of<real_t>:: prepare()
            {
                get_ready(compare);
            }

            static inline size_t just_load(ios::istream    &fp,
                                           sequence<real_t> &x,
                                           const size_t      ix,
                                           sequence<real_t> &y,
                                           const size_t      iy,
                                           const size_t      nskip)
            {
                ios::ascii::loader ld;
                ld(x,ix);
                ld(y,iy);
                return ld.load(fp,nskip);
            }



            template <>
            size_t real_sample_of<real_t>:: load(ios::istream &fp,
                                                 const size_t  ix,
                                                 const size_t  iy,
                                                 const size_t  nskip)
            {
                vector<real_t> x(128,as_capacity);
                vector<real_t> y(128,as_capacity);
                const size_t   n = just_load(fp,x,ix,y,iy,nskip);
                make(n);
                assert(n == mutual_size() );
                assert(n == dimension());
                for(size_t i=n;i>0;--i)
                {
                    coerce(abscissa[i]) = x[i];
                    coerce(ordinate[i]) = y[i];
                }

                prepare();

                return dimension();
            }
        }

    }

}


