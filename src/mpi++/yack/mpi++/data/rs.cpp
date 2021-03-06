#include "yack/mpi++/mpi.hpp"
#include "yack/type/complex.hpp"
#include "yack/mpi++/data/lcplx.hxx"
#include "yack/mpi++/data/string.hxx"
#include "yack/mpi++/data/srz.hxx"
#include "yack/apex.hpp"


namespace yack
{


    namespace __mpi
    {

        namespace
        {
            template <typename T>
            class native_data_io : public data_io_for<T>
            {
            public:

                inline virtual ~native_data_io() throw() {}

                inline explicit native_data_io(const data_types &native) :
                data_io_for<T>(),
                mdt( native.get(this->tid)  )
                {
                }

                virtual void send(const mpi   &MPI,
                                  const void  *ptr,
                                  const size_t num,
                                  const int    dst,
                                  const int    tag) const
                {
                    MPI.Send(ptr,num,mdt.info,mdt.size,dst,tag);
                }

                virtual void recv(const mpi   &MPI,
                                  void        *ptr,
                                  const size_t num,
                                  const int    src,
                                  const int    tag) const
                {
                    MPI.Recv(ptr,num,mdt.info,mdt.size,src,tag);
                }

                const data_type mdt;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(native_data_io);
            };
        }





        data_rs:: ~data_rs() throw()
        {
        }

#define YACK_MPI_RS(TYPE) do {                            \
/**/ const io_ptr p = new native_data_io< TYPE >(native); \
/**/ (void) db.insert(p);                                 \
} while(false)

        void data_rs:: make_built_in(const data_types &native)
        {
            YACK_MPI_RS(uint8_t);
            YACK_MPI_RS(uint16_t);
            YACK_MPI_RS(uint32_t);
            YACK_MPI_RS(uint64_t);

            YACK_MPI_RS(int8_t);
            YACK_MPI_RS(int16_t);
            YACK_MPI_RS(int32_t);
            YACK_MPI_RS(int64_t);

            YACK_MPI_RS(char);
            YACK_MPI_RS(short);
            YACK_MPI_RS(int);
            YACK_MPI_RS(long);
            YACK_MPI_RS(long long);

            YACK_MPI_RS(unsigned char);
            YACK_MPI_RS(unsigned short);
            YACK_MPI_RS(unsigned);
            YACK_MPI_RS(unsigned long);
            YACK_MPI_RS(unsigned long long);

            YACK_MPI_RS(float);
            YACK_MPI_RS(double);
            YACK_MPI_RS(long double);

            YACK_MPI_RS(complex<float>);
            YACK_MPI_RS(complex<double>);

            YACK_MPI_RS(size_t);
            YACK_MPI_RS(ptrdiff_t);

        }

        void data_rs:: make_cxx_from(const data_types &native)
        {
            declare( new lcplx_io(native)  );
            declare( new string_io()       );
            declare( new srz_io_for<apn>() );
            declare( new srz_io_for<apz>() );
            declare( new srz_io_for<apq>() );
        }

        data_rs:: data_rs(const data_types &native) : db()
        {
            make_built_in(native);
            make_cxx_from(native);
        }

        void data_rs:: declare(data_io *ops)
        {
            assert(ops);
            const io_ptr p = ops;
            if(!db.insert(p))
            {
                const string &name = p->tid.name();
                throw exception("%s multiple data_io for <%s>", mpi::call_sign, name() );
            }
        }


        const data_io & data_rs:: get(const rtti &tid) const
        {
            const be_address key(tid);
            const io_ptr *ppIO = db.search(key);
            if(!ppIO)
            {
                const string &name = tid.name();
                throw exception("%s no Recv/Send for <%s>",mpi::call_sign,name());
            }
            return **ppIO;
        }

    }

}


