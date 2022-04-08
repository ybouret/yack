//! \file

#ifndef YACK_MPICXX_DATA_IO_INCLUDED
#define YACK_MPICXX_DATA_IO_INCLUDED 1


#include "yack/system/rtti.hpp"


namespace yack
{

    class mpi;

    namespace __mpi
    {
        class data_io : public object
        {
        public:
            virtual ~data_io() throw();

            virtual void send(const mpi &, const void *addr) const;

        protected:

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(data_io);
        };
    }

}
#endif

