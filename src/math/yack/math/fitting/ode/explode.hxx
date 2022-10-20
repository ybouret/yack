
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
                rates(dYdt,t,Y,*p_aorg,*p_vars);
            }



            template <> explODE<real_t>:: explODE(const app_ptr &usr,
                                                  const size_t   dim) :
            seq_type(),
            arr_type(dim),
            app( usr ),
            diffeq(this, & explODE<real_t>::call_ode),
            p_aorg(NULL),
            p_vars(NULL)
            {
            }

            template <>
            real_t explODE<real_t>:: run(const real_t t0, const real_t t1, const parameters &aorg, const variables &vars)
            {
                // prepare global
                assert(NULL==p_aorg);
                assert(NULL==p_vars);
                const temporary<const parameters *> keepAorg(p_aorg,&aorg);
                const temporary<const variables  *> keepVars(p_vars,&vars);

                // prepare local
                app_type         &odeint = *app;
                writable<real_t> &Y      = *this;
                real_t            h      = std::abs(t1-t0);
                odeint(Y,t0,t1,h,diffeq,NULL);
                return query();
            }
            
            template <>
            real_t explODE<real_t>:: on_start(const real_t ini, const parameters &aorg, const variables &vars)
            {
                return run( setup(*this,aorg,vars), ini, aorg, vars);
            }

            template <>
            real_t explODE<real_t>:: on_reach(const real_t ini, const real_t end, const parameters &aorg, const variables &vars)
            {
                return run( ini, end, aorg, vars);
            }




        }
    }
}

