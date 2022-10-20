//! \file

#ifndef YACK_FITTING_EXPLODE_INCLUDED
#define YACK_FITTING_EXPLODE_INCLUDED 1

#include "yack/math/fitting/sequential.hpp"
#include "yack/math/ode/appliance.hpp"
#include "yack/math/ode/explicit/rkck.hpp"
#include "yack/sequence/cxx-array.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {

            //__________________________________________________________________
            //
            //
            //! assembling ODE/Solver to make a sequential fit function
            //
            //__________________________________________________________________
            template <typename T> class explODE :
            public sequential<T,T>,
            public cxx_array<T>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef sequential<T,T>               seq_type;   //!< alias to base class
                typedef ode::appliance<T>             app_type;   //!< alias to appliance
                typedef typename app_type::pointer    app_ptr;    //!< alias
                typedef cxx_array<T>                  arr_type;   //!< alias
                typedef typename seq_type::parameters parameters; //!< alias
                typedef fitting::variables            variables;  //!< alias
                typedef typename app_type::equation   equation;   //!< alias
                typedef typename app_type::callback   callback;   //!< alias

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                //! phase space to real-data
                virtual T    query(const readable<T> &) const = 0;
                
                //! setup initial phase space and return starting abscissa
                virtual T    setup(writable<T> &) const = 0;

                //! return maximal step
                virtual T         delta() const = 0;

                //! differemtial, parametric equations
                virtual void rates(writable<T>       &dYdt,
                                   T                  t,
                                   const readable<T> &Y) = 0;

                //! get callback, default to NULL
                virtual callback *check() throw();

                template <typename PARAM> inline
                T get(const PARAM &param) const
                {
                    assert(p_aorg);
                    assert(p_vars);
                    return (*p_vars)(*p_aorg,param);
                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                app_ptr      app; //!< shared appliance
                

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~explODE() throw(); //!< cleanup


            protected:
                explicit explODE(const app_ptr &usr,
                                 const size_t   dim); //!< setup


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(explODE);
                equation           diffeq; //!< differential equation wrapper
                const parameters  *p_aorg; //!< pointer to current parameters
                const variables   *p_vars; //!< pointer to current variables


                //! wrapper for differential solver
                void      call_ode(writable<T> &dYdt, T t, const readable<T> &Y);

                //! return run( setup(*this,aorg,vars), ini, aorg, vars)
                virtual T on_start(const T ini, const parameters &aorg, const variables &vars);

                //! return run(ini,end,aorg,vars)
                virtual T on_reach(const T ini, const T end, const parameters &aorg, const variables &vars);

                //! integrate equation
                T         run(const T t0, const T t1);

            };


        }

    }
}


#endif

