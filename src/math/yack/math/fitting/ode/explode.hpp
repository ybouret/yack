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

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                

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
                
                virtual T on_start(const T ini, const parameters &A, const variables &vars);
                virtual T on_reach(const T ini, const T end, const parameters &A, const variables &vars);

            };


        }

    }
}


#endif

