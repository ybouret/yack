//! \file

#ifndef YACK_FIT_LEAST_SQUARES_DATA_INCLUDED
#define YACK_FIT_LEAST_SQUARES_DATA_INCLUDED 1

#include "yack/math/fit/least-squares/lambda.hpp"
#include "yack/math/algebra/lu.hpp"
#include "yack/math/fcn/derivative.hpp"
#include "yack/sequence/arrays.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/large-object.hpp"

namespace yack
{
    namespace math
    {
        namespace fit
        {
            class variables;

            //__________________________________________________________________
            //
            //
            //! info for least squares
            //
            //__________________________________________________________________
            class least_squares_info : public large_object
            {
            public:
                static const char clid[];              //!< least-square
                explicit least_squares_info() throw(); //!< setup
                virtual ~least_squares_info() throw(); //!< cleanup

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(least_squares_info);
            };

            //__________________________________________________________________
            //
            //
            //! data for least squares, depending only on ordinates type
            //
            //__________________________________________________________________
            template <typename ORDINATE>
            class least_squares_data :  public least_squares_info, public arrays_of<ORDINATE>
            {
            public:
                //______________________________________________________________
                //
                // definitions
                //______________________________________________________________
                typedef arrays_of<ORDINATE>                      tableaux;   //!< alias
                typedef typename arrays_of<ORDINATE>::array_type array_type; //!< alias
                typedef lu<ORDINATE>                             LU;         //!< alias


                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~least_squares_data() throw(); //!< cleanup

            protected:
                explicit least_squares_data();         //!< setup

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(least_squares_data);

            public:
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                ORDINATE               D2;   //!< last D2
                array_type            &step; //!< last step
                array_type            &atry; //!< trial parameters
                matrix<ORDINATE>       curv; //!< modified matrix
                derivative<ORDINATE>   drvs; //!< derivative
                auto_ptr<LU>           algo; //!< algebra
                const lambda<ORDINATE> lam;  //!< pre-computed coefficients
                const unit_t           pmin; //!< lam.lower
                const unit_t           pmax; //!< lam.upper
                const ORDINATE         ftol; //!< lam[pmin+1]
                const ORDINATE         dtol; //!< lam[pmin/2]

            protected:
                bool                   shrinking; //!< per cycle indicator

            public:
                bool                   verbose;   //!< verbosity for tracing

            protected:


                //______________________________________________________________
                //
                //! test step not zero
                //______________________________________________________________
                bool has_moved() const throw();

                //______________________________________________________________
                //
                //! test convergence w.r.t last step
                //______________________________________________________________
                bool converged(const readable<ORDINATE> &atmp) const throw();

                //______________________________________________________________
                //
                //! initialize for a cycle: curvature, mutual size and algo
                //______________________________________________________________
                void initialize(const size_t npar);

                //______________________________________________________________
                //
                //! try to increase p to shrink step
                //______________________________________________________________
                bool shrink(unit_t &p) throw();

                //______________________________________________________________
                //
                //! try to compute look-up curvature
                //______________________________________________________________
                bool compute_curvature(unit_t                 &p,
                                       const matrix<ORDINATE> &hess,
                                       const variables        &vars,
                                       const readable<bool>   &used);

                //______________________________________________________________
                //
                //! try to optimize descent step
                //______________________________________________________________
                ORDINATE optimize(real_function<ORDINATE> &g,
                                  const ORDINATE           g0,
                                  const ORDINATE           g1) const;

                //______________________________________________________________
                //
                //! compute erros on parameters
                //______________________________________________________________
                void compute_errors(writable<ORDINATE>   &aerr,
                                    matrix<ORDINATE>     &covm,
                                    const variables      &vars,
                                    const readable<bool> &used,
                                    const size_t          ndat);
                
            };

            //! helper to display least squares fit messages
#define YACK_LSF_PRINTLN(MSG) do { if(verbose) { std::cerr << MSG << std::endl; } } while(false)
            
        }
    }
}

#endif

