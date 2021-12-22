//! \file

#ifndef YACK_RANDOMIZED_METRICS_INCLUDED
#define YACK_RANDOMIZED_METRICS_INCLUDED 1

#include "yack/randomized/u32-to.hpp"

namespace yack
{
    namespace randomized
    {
        
        //______________________________________________________________________
        //
        //
        //! base class for metrics
        //
        //______________________________________________________________________
        class metrics
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit metrics(const uint32_t umax) throw(); //!< setup
            virtual ~metrics() throw();                    //!< cleanup

            const uint32_t            m; //!< user's maximum provided value
            const u32_to<float>       f; //!< float       metrics
            const u32_to<double>      d; //!< double      metrics
            const u32_to<long double> l; //!< long double metrics

            //! for float|double|long double
            template <typename T> T unit_(const uint32_t u) const throw();

            //! for float|double|long double
            template <typename T> T symm_(const uint32_t u) const throw();


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(metrics);

        };



        

    }
}

#endif

