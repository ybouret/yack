//! \file

#ifndef YACK_MPICXX_INCLUDED
#define YACK_MPICXX_INCLUDED 1

#include "yack/mpi++/data-types.hpp"
#include "yack/singleton.hpp"
#include "yack/exception.hpp"
#include "yack/string.hpp"

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
        // type -> MPI_Datatype
        //______________________________________________________________________
        MPI_Datatype DataType(const rtti &) const; //!< get matching datatype

        //! helper to get matching datatype
        template <typename T> inline
        MPI_Datatype DataType() const
        {
            static const MPI_Datatype res = DataType( rtti::use(typeid(T)) );
            return res;
        }

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


        void SYN(const int dst) const; //!< send syn_ack
        void ACK(const int src) const; //!< recv syn_ack

        //! low-level sending of integral types
        template <typename T> inline
        void Send(const T     *arr,
                  const size_t num,
                  const int    dst,
                  const int    tag = io_tag) const
        {
            static const MPI_Datatype mdt = DataType( rtti::use(typeid(T)) );
            Send(arr,num,mdt,dst,tag);
        }

        //! low-level sending of integral types
        template <typename T> inline
        void Recv(T           *arr,
                  const size_t num,
                  const int    src,
                  const int    tag = io_tag) const
        {
            static const MPI_Datatype mdt = DataType( rtti::use(typeid(T)) );
            Recv((void*)arr,num,mdt,src,tag);
        }

        //! default type sending
        template <typename T> inline
        void Send(const T   &obj,
                  const int  dst,
                  const int  tag = io_tag) const
        {
            Send<T>(&obj,1,dst,tag);
        }


        //! default type receiving
        template <typename T> inline
        T Recv(const int src,
               const int tag = io_tag) const
        {
            T res(0);
            Recv(&res,1,src,tag);
            return res;
        }



        //______________________________________________________________________
        //
        // helpers
        //______________________________________________________________________
        void primary_sync() const; //!< all SYN/ACK
        void replica_wait() const; //!< ACK(0)
        void replica_done() const; //!< SYN(0)

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const int    rank;         //!< MPI_Comm_rank(MPI_COMM_WORLD)
        const int    size;         //!< MPI_Comm_size(MPI_COMM_WORLD)
        const bool   primary;      //!< 0==rank
        const bool   replica;      //!< 0!=rank
        const int    threading;    //!< MPI Thread Level
        const string name;         //!< size.rank

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(mpi);
        friend class singleton<mpi>;
        explicit mpi();
        virtual ~mpi() throw();
        const __mpi::data_types native;
    };

    //! partial implementation
    template <> void mpi:: Send<string>(const string &,const int, const int) const;

    //! partial implementation
    template <> string mpi:: Recv<string>(const int, const int) const;


    //__________________________________________________________________________
    //
    //! perform in-order operations
    //__________________________________________________________________________
#define YACK_SYNCHRONIZE(WITH_MPI,CODE)     \
/**/ do { const mpi & THE_MPI = WITH_MPI;   \
/**/     if(THE_MPI.primary) {              \
/**/         do { CODE; } while(false);     \
/**/         THE_MPI.primary_sync();        \
/**/     } else { assert(THE_MPI.replica);  \
/**/         THE_MPI.replica_wait();        \
/**/         do { CODE; } while(false);     \
/**/         THE_MPI.replica_done();        \
/**/     }                                  \
/**/ } while(false)

}


#endif

