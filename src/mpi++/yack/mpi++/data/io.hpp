//! \file

#ifndef YACK_MPICXX_DATA_IO_INCLUDED
#define YACK_MPICXX_DATA_IO_INCLUDED 1


#include "yack/system/rtti.hpp"


namespace yack
{

    class mpi;

    namespace __mpi
    {
        //! I/O for data
        class data_io : public object
        {
        public:
            virtual ~data_io() throw(); //!< cleanup

        protected:

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(data_io);
        };
    }

}
#endif

