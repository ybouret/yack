//! \file

#ifndef YACK_FITTING_EXPLODE_INCLUDED
#define YACK_FITTING_EXPLODE_INCLUDED 1

#include "yack/math/fitting/sequential.hpp"
#include "yack/math/ode/appliance.hpp"
#include "yack/math/ode/explicit/rkck.hpp"

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
            template <typename T> class explODE : public sequential<T,T>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef sequential<T,T>               seq_type;   //!< alias to base class
                typedef ode::appliance<T>             app_type;   //!< alias to appliance
                typedef typename app_type::pointer    app_ptr;    //!< alias
                typedef vector<T>                     vec_type;   //!< alias
                typedef typename seq_type::parameters parameters; //!< alias

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! TODO
                virtual void initialize(const T ini, const parameters &A, const variables &vars) = 0;



                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                app_ptr  app; //!< shared appliance
                vec_type Y;   //!< phase space




                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~explODE() throw(); //!< cleanup


            protected:
                explicit explODE(const app_ptr &usr); //!< setup


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(explODE);
                
                virtual T on_start(const T ini, const parameters &A, const variables &vars);
                virtual T on_reach(const T ini, const T end, const parameters &A, const variables &vars);

            };


        }

    }
}


#endif

