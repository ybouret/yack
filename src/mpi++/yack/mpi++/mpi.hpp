//! \file

#ifndef YACK_MPICXX_INCLUDED
#define YACK_MPICXX_INCLUDED 1

#include "yack/singleton.hpp"
#include "yack/exception.hpp"

#define OMPI_SKIP_MPICXX 1 //!< helper for OpenMPI
#include <mpi.h>

namespace yack
{

    //__________________________________________________________________________
    //
    //! macro to trigger an exception upon MPI function failure
    //__________________________________________________________________________
#define YACK_MPI_CALL(CODE) do { const int ret = CODE; if(MPI_SUCCESS!=ret) throw mpi::exception(ret,"in %s",#CODE); } while(false)

    //__________________________________________________________________________
    //
    //
    //! MPI wrappers
    //
    //__________________________________________________________________________
    class mpi : public singleton<mpi>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        static const at_exit::longevity life_time = 2000; //!< life time
        static const char  * const      call_sign;        //!< call sign

        //______________________________________________________________________
        //
        //! specific exception
        //______________________________________________________________________
        class exception : public yack::exception
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! what=MPI error message, when=formatted string
            explicit exception(const int err,const char *fmt,...) throw() YACK_PRINTF_CHECK(3,4);
            exception(const exception &)      throw(); //!< copy
            virtual ~exception()              throw(); //!< cleanup
            virtual  const char *what() const throw(); //!< internal message

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const int code; //!< MPI error code
        private:
            YACK_DISABLE_ASSIGN(exception);
            char data[MPI_MAX_ERROR_STRING];
        };


        //______________________________________________________________________
        //
        // initialization
        //______________________________________________________________________
        static mpi & Init(int &argc, char ** &argv); //!< initialize


        const int    rank;         //!< MPI_Comm_rank(MPI_COMM_WORLD)
        const int    size;         //!< MPI_Comm_size(MPI_COMM_WORLD)
        const int    threading;    //!< MPI Thread Level
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(mpi);
        friend class singleton<mpi>;
        explicit mpi();
        virtual ~mpi() throw();
    };


}


#endif

