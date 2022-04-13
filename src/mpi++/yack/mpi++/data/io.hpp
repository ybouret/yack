//! \file

#ifndef YACK_MPICXX_DATA_IO_INCLUDED
#define YACK_MPICXX_DATA_IO_INCLUDED 1


#include "yack/system/rtti.hpp"
#include "yack/associative/be-addr.hpp"


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
            virtual void send(mpi &, const void *addr, const int dst, const int tag) const = 0;
            virtual void recv(mpi &, void *addr,       const int src, const int tag) const = 0;

            const rtti              &tid;
            const memory::ro_buffer &key() const throw();

        protected:
            explicit data_io(const rtti &) throw(); //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(data_io);
            const be_address bek;
        };
    }

}
#endif

