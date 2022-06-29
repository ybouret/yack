//! \file

#ifndef YACK_SYNC_THREAD_INCLUDED
#define YACK_SYNC_THREAD_INCLUDED 1

#include "yack/concurrent/quark/threadable.hpp"
#include "yack/setup.hpp"

namespace yack
{
    namespace concurrent
    {
        //______________________________________________________________________
        //
        //
        // forward declaration
        //
        //______________________________________________________________________
        namespace quark
        {
            class thread;
        }

        //______________________________________________________________________
        //
        //
        //! thread creation and startup
        /**
         The methods are directly implemented in thread-api.hxx
         */
        //
        //______________________________________________________________________
        class thread : public quark::threadable
        {
        public:
            static bool verbose; //!< system wide verbosity for threads ops
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit thread(procedure proc, void *args); //!< initialize
            virtual ~thread() throw();                   //!< wait and destruct

            void        assign(const size_t cpu, const char *who=NULL);         //!< assign to cpu, with optional verbosity
            static void assign_current(const size_t cpu, const char *who=NULL); //!< assign current thread to cpu, with optional verbosity

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(thread);
            quark::thread *impl;
        };

        //! verbose info
#define YACK_THREAD_PRINTLN(MSG) do { \
/**/  if(yack::concurrent::thread::verbose) (std::cerr << MSG << std::endl).flush();\
/**/  } while(false)

    }
}

#endif
