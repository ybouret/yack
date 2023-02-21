
//! \file

#ifndef YACK_MPICXX_DATA_TYPE_INCLUDED
#define YACK_MPICXX_DATA_TYPE_INCLUDED 1

#include "yack/setup.hpp"

#define OMPI_SKIP_MPICXX 1 //!< helper for OpenMPI
#include <mpi.h>

namespace yack
{

    namespace __mpi
    {

        //______________________________________________________________________
        //
        //! wrapper for type+sizeof
        //______________________________________________________________________
        class data_type
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            data_type(const MPI_Datatype, const unsigned) noexcept; //!< setup
            ~data_type()                                  noexcept; //!< cleanup
            data_type(const data_type &)                  noexcept; //!< copy

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const MPI_Datatype info; //!< uuid
            const unsigned     size; //!< sizeof

        private:
            YACK_DISABLE_ASSIGN(data_type);
        };
    }
}

#endif

