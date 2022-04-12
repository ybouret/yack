//! \file

#ifndef YACK_MPICXX_INCLUDED
#define YACK_MPICXX_INCLUDED 1

#include "yack/mpi++/data/types.hpp"
#include "yack/singleton.hpp"
#include "yack/exception.hpp"
#include "yack/string.hpp"
#include "yack/system/wtime.hpp"
#include "yack/check/printf.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //! macro to trigger an exception upon MPI function failure
    //__________________________________________________________________________
#define YACK_MPI_CALL(CODE) do { const int ret = CODE; if(MPI_SUCCESS!=ret) throw mpi::exception(ret,"in %s",#CODE); } while(false)

    //__________________________________________________________________________
    //
    //! initialize local timing
    //__________________________________________________________________________
#define YACK_MPI_TMX_INIT()  const uint64_t __mark = wtime::ticks()

    //__________________________________________________________________________
    //
    //! register local timing and send/recv bytes
    //__________________________________________________________________________
#define YACK_MPI_TMX_DONE(which,BYTES)           \
const uint64_t __done = wtime::ticks() - __mark; \
tmx &          __info = coerce(which##_tmx);     \
__info.ticks += __done;                          \
__info.bytes += (BYTES)

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

        //! timings structure
        struct tmx {
            uint64_t ticks; //!< ellapsed  ticks
            uint64_t bytes; //!< processed bytes
        };


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
        const __mpi::data_type & DataType(const rtti &) const; //!< get matching datatype

        //! helper to get matching datatype
        template <typename T> inline
        const __mpi::data_type &DataType() const
        {
            static const __mpi::data_type &res = DataType( rtti::use(typeid(T)) );
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
                  const unsigned     bpi,
                  const int          dst,
                  const int          tag) const;

        //! low-level blocking recv on MPI_COMM_WORLD
        void Recv(void              *buf,
                  const size_t       num,
                  const MPI_Datatype tid,
                  const unsigned     bpi,
                  const int          src,
                  const int          tag) const;


        void SYN(const int dst) const; //!< send syn_ack
        void ACK(const int src) const; //!< recv syn_ack

        //! low-level sending of integral types
        template <typename T> inline
        void Send(const T     *arr,
                  const size_t num,
                  const int    dst,
                  const int    tag) const
        {
            static const __mpi::data_type mdt = DataType( rtti::use(typeid(T)) );
            Send(arr,num,mdt.info,mdt.size,dst,tag);
        }

        //! low-level sending of integral types
        template <typename T> inline
        void Recv(T           *arr,
                  const size_t num,
                  const int    src,
                  const int    tag = io_tag) const
        {
            static const __mpi::data_type mdt = DataType( rtti::use(typeid(T)) );
            Recv((void*)arr,num,mdt.info,mdt.size,src,tag);
        }

        //! default type sending
        template <typename T> inline
        void Send(const T   &obj,
                  const int  dst,
                  const int  tag) const
        {
            Send<T>(&obj,1,dst,tag);
        }


        //! default type receiving
        template <typename T> inline
        T Recv(const int src,
               const int tag) const
        {
            T res(0);
            Recv<T>(&res,1,src,tag);
            return res;
        }

        //______________________________________________________________________
        //
        //! printf on primary
        //______________________________________________________________________
        void Printf(FILE *,const char *fmt,...) const YACK_PRINTF_CHECK(3,4);

        //______________________________________________________________________
        //
        // helpers
        //______________________________________________________________________
        void primary_sync() const; //!< all SYN/ACK
        void replica_wait() const; //!< ACK(0)
        void replica_done() const; //!< SYN(0)

        void tmx_init() throw();   //!< reset timings

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
        const tmx    send_tmx;     //!< send timings
        const tmx    recv_tmx;     //!< recv timings
        
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

