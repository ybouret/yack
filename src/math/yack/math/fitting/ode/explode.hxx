
namespace yack
{
    namespace math
    {

        namespace fitting
        {

            template <> explODE<real_t>:: ~explODE() throw() {}

            
            template <>
            void explODE<real_t>:: call_ode(writable<real_t> &dYdt, real_t t, const readable<real_t> &Y)
            {
                assert(p_aorg);
                assert(p_vars);
                rates(dYdt,t,Y);
            }


            template <>
            typename explODE<real_t>::callback * explODE<real_t>:: check() throw()
            {
                return NULL;
            }

            template <> explODE<real_t>:: explODE(const app_ptr &usr,
                                                  const size_t   dim) :
            seq_type(),
            arr_type(dim),
            app(usr),
            diffeq(this, & explODE<real_t>::call_ode),
            p_aorg(NULL),
            p_vars(NULL)
            {
            }

            template <>
            real_t explODE<real_t>:: run(const real_t t0, const real_t t1)
            {
               
                app_type         &odeint = *app;
                writable<real_t> &Y      = *this;
                const real_t      dt_max = std::abs( delta() );
                const real_t      width  = t1-t0;
                const real_t      dt_cur = std::abs(width);
                if(dt_max<=0) throw libc::exception(EINVAL,"delta()<=0 in explODE");

                if(dt_cur<=dt_max)
                {
                    // single step
                    real_t h = dt_cur/2;
                    odeint(Y,t0,t1,h,diffeq,check());
                }
                else
                {
                    // multiple steps
                    assert(dt_max>0);
                    assert(dt_cur>dt_max);

                    real_t        h = dt_max/2;
                    size_t        n = std::ceil(dt_cur/dt_max);
                    callback     *cb = check();
                    for(size_t i=1;i<=n;++i)
                    {
                        const real_t t_ini = t0 + ((i-1) *width)/n;
                        const real_t t_end = t0 + (i*width)/n;
                        odeint(Y,t_ini,t_end,h,diffeq,cb);
                    }

                }
                return query(Y);
            }
            
            template <>
            real_t explODE<real_t>:: on_start(const real_t ini, const parameters &aorg, const variables &vars)
            {
                // prepare global
                assert(NULL==p_aorg);
                assert(NULL==p_vars);
                const temporary<const parameters *> keepAorg(p_aorg,&aorg);
                const temporary<const variables  *> keepVars(p_vars,&vars);
                const real_t org = setup(*this);
                return run(org,ini);
            }

            template <>
            real_t explODE<real_t>:: on_reach(const real_t ini, const real_t end, const parameters &aorg, const variables &vars)
            {
                // prepare global
                assert(NULL==p_aorg);
                assert(NULL==p_vars);
                const temporary<const parameters *> keepAorg(p_aorg,&aorg);
                const temporary<const variables  *> keepVars(p_vars,&vars);
                return run(ini,end);
            }




        }
    }
}

