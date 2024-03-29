
#ifndef YACK_MPICXX_DATA_SRZ_INCLUDED
#define YACK_MPICXX_DATA_SRZ_INCLUDED 1

#include "yack/mpi++/data/io.hpp"
#include "yack/mpi++/data/types.hpp"
#include "yack/ios/ovstream.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/ios/serializable.hpp"
#include "yack/ios/imstream.hpp"
#include "yack/type/constant.hpp"
#include "yack/exception.hpp"

namespace yack
{
    class mpi;

    namespace __mpi
    {

        //! data I/O for serializable classes
        class srz_io : public data_io
        {
        public:
            typedef ios::ovstream<memory::dyadic> io_buffer; //!< alias
            virtual ~srz_io() noexcept;                       //!< cleanup

        protected:
            explicit srz_io(const rtti &uuid); //!< setup
            mutable io_buffer io;              //!< internal buffer

            //! use serializable interface to fill and send buffer
            void send1(const mpi               &MPI,
                       const ios::serializable &srz,
                       const int                dst,
                       const int                tag) const;

            //! recv a size_t
            static size_t recv_size(const mpi &MPI, const int src, const int tag);

            //! recv a buffer within io
            void          recv_buff(const mpi &MPI, const size_t len, const int src, const int tag) const;

            //! check constructed length
            void          check_len(const size_t len, const size_t cm) const;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(srz_io);

        };

        //! specific data I/O
        template <typename SERIALIZABLE>
        class srz_io_for : public srz_io
        {
        public:
            //! cleanup
            inline virtual ~srz_io_for() noexcept {}

            //! initialize type and constant default field
            inline explicit srz_io_for() :
            srz_io( rtti::use<SERIALIZABLE>() ),
            zero()
            {
            }

            //! loop over item
            virtual void send(const mpi   &MPI,
                              const void  *ptr,
                              const size_t num,
                              const int    dst,
                              const int    tag) const
            {
                const SERIALIZABLE *s = static_cast<const SERIALIZABLE *>(ptr);
                for(size_t i=num;i>0;--i)
                {
                    send1(MPI,*(s++),dst,tag);
                }
            }

            //! recv using the static construct() method
            virtual void recv(const mpi   &MPI,
                              void        *ptr,
                              const size_t num,
                              const int    src,
                              const int    tag) const
            {
                SERIALIZABLE *s = static_cast<SERIALIZABLE *>(ptr);
                for(size_t i=num;i>0;--i)
                {
                    const size_t  len = recv_size(MPI,src,tag);
                    SERIALIZABLE &tgt = *(s++);
                    if(len>0)
                    {
                        recv_buff(MPI,len,src,tag);
                        size_t        cm = 0;
                        ios::imstream fp(io.buffer(),len);
                        tgt = SERIALIZABLE::construct(fp,cm);
                        check_len(len,cm);
                    }
                    else
                    {
                        tgt = *zero;
                    }
                }
            }

            const const_field<SERIALIZABLE> zero; //!< default field

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(srz_io_for);
        };


    }

}

#endif
