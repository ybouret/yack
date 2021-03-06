//! \file

#ifndef YACK_SYNC_LOOP_INCLUDED
#define YACK_SYNC_LOOP_INCLUDED 1

#include "yack/concurrent/loop/runnable.hpp"
#include "yack/concurrent/assembly.hpp"
#include "yack/counted.hpp"

namespace yack
{
    namespace concurrent
    {

        //______________________________________________________________________
        //
        //
        //! interface for Single Instruction Multiple Data
        //
        //______________________________________________________________________
        class loop : public object, public counted, public assembly
        {
        public:
            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual lockable   &access()          throw() = 0; //!< to sync
            virtual const char *family()    const throw() = 0; //!< for info
            virtual void operator()(kernel,void*) throw() = 0; //!< call kernel once

            //__________________________________________________________________
            //
            // non-virtual interface
            //__________________________________________________________________
            void run(runnable &obj) throw(); //!< obj.run(...)

            //! compute efficiency
            double efficiency(const double seq_rate, const double par_rate) const throw();

            //__________________________________________________________________
            //
            // data management
            //__________________________________________________________________

          


            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~loop() throw(); //!< cleanup
        protected:
            explicit loop() throw(); //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(loop);
        };

    }

}

#endif
