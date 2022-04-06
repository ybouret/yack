//! \file

#ifndef YACK_MPICXX_INCLUDED
#define YACK_MPICXX_INCLUDED 1

#include "yack/mpi++/data-types.hpp"
#include "yack/singleton.hpp"
#include "yack/exception.hpp"


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
        static const int                io_tag    = 0x07; //!< default IO tag
        static const uint8_t            syn_ack   = 0xff; //!< syn/ack byte

        
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

        //______________________________________________________________________
        //
        // point to point
        //______________________________________________________________________

        //! low-level blocking send on MPI_COMM_WORLD
        void Send(const void        *buf,
                  const size_t       num,
                  const MPI_Datatype tid,
                  const int          dst,
                  const int          tag = io_tag) const;

        //! low-level blocking recv on MPI_COMM_WORLD
        void Recv(void              *buf,
                  const size_t       num,
                  const MPI_Datatype tid,
                  const int          src,
                  const int          tag = io_tag) const;


        void SYN(const int dst, const int tag = io_tag) const; //!< send syn_ack
        void ACK(const int src, const int tag = io_tag) const; //!< recv syn_ack

        MPI_Datatype DataType(const rtti &) const;
        template <typename T> inline
        MPI_Datatype DataType() const
        {
            static const MPI_Datatype res = DataType( rtti::use(typeid(T)) );
            return res;
        }

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const int    rank;         //!< MPI_Comm_rank(MPI_COMM_WORLD)
        const int    size;         //!< MPI_Comm_size(MPI_COMM_WORLD)
        const int    threading;    //!< MPI Thread Level
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(mpi);
        friend class singleton<mpi>;
        explicit mpi();
        virtual ~mpi() throw();
        __mpi::data_types dtdb;
    };


}


#endif

