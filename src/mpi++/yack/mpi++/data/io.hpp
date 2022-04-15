//! \file

#ifndef YACK_MPICXX_DATA_IO_INCLUDED
#define YACK_MPICXX_DATA_IO_INCLUDED 1


#include "yack/mpi++/data/type.hpp"
#include "yack/associative/be-addr.hpp"
#include "yack/system/rtti.hpp"

namespace yack
{

    class mpi;

    namespace __mpi
    {
        //______________________________________________________________________
        //
        //! I/O for data
        //______________________________________________________________________
        class data_io : public object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const memory::ro_buffer &key() const throw(); //!< key for ptr

            //! sending contiguous objects
            virtual void send(const mpi   &,
                              const void  *ptr,
                              const size_t num,
                              const int    dst,
                              const int    tag) const = 0;

            //! recving contiguous objects
            virtual void recv(const mpi    &,
                              void        *ptr,
                              const size_t num,
                              const int    src,
                              const int    tag) const = 0;

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const rtti              &tid; //!< type id

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~data_io() throw(); //!< cleanup

        protected:
            explicit data_io(const rtti &) throw(); //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(data_io);
            const be_address bek;
        };

        //______________________________________________________________________
        //
        //! helper
        //______________________________________________________________________
        template <typename T>
        class data_io_for : public data_io
        {
        public:
            typedef data_io_for<T> self_data_io; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! cleanup
            inline virtual ~data_io_for() throw() {}

        protected:
            //! setup
            inline explicit data_io_for() : data_io( rtti::use<T>() ) {}
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(data_io_for);
        };

    }

}
#endif

