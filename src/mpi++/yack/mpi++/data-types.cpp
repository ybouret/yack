#include "yack/mpi++/data-types.hpp"
#include "yack/associative/be-addr.hpp"
#include "yack/exception.hpp"
#include "yack/string.hpp"
#include "yack/type/complex.hpp"

namespace yack
{
    namespace __mpi
    {



        data_types:: data_types() : data_type_tree()
        {
            data_types &self = *this;
            self.in<uint8_t>(MPI_UINT8_T);
            self.in<uint16_t>(MPI_UINT16_T);
            self.in<uint32_t>(MPI_UINT32_T);
            self.in<uint64_t>(MPI_UINT64_T);

            self.in<int8_t>(MPI_INT8_T);
            self.in<int16_t>(MPI_INT16_T);
            self.in<int32_t>(MPI_INT32_T);
            self.in<int64_t>(MPI_INT64_T);

            self.in<char>(MPI_CHAR);
            self.in<short>(MPI_SHORT);
            self.in<int>(MPI_INT);
            self.in<long int>(MPI_LONG_INT);
            self.in<long long int>(MPI_LONG_LONG_INT);


            self.in<unsigned char>(MPI_UNSIGNED_CHAR);
            self.in<unsigned short>(MPI_UNSIGNED_SHORT);
            self.in<unsigned int>(MPI_UNSIGNED);
            self.in<unsigned long>(MPI_UNSIGNED_LONG);
            self.in<unsigned long long>(MPI_UNSIGNED_LONG_LONG);

            self.in<float>(MPI_FLOAT);
            self.in<double>(MPI_DOUBLE);
            self.in<long double>(MPI_LONG_DOUBLE);

            self.in< complex<float>  >(MPI_COMPLEX);
            self.in< complex<double> >(MPI_DOUBLE_COMPLEX);

            //self.in<bool>(MPI_CXX_BOOL);

            (void) self( rtti::use<size_t>()    );
            (void) self( rtti::use<ptrdiff_t>() );

        }
        

        data_types:: ~data_types() throw()
        {

        }

        void data_types:: make(const rtti        &tid,
                               const MPI_Datatype mdt)
        {
            const be_address key(tid);
            (void) insert(mdt,key.begin(),key.measure());
        }


        const MPI_Datatype * data_types:: query(const rtti &tid) const throw()
        {
            const be_address key(tid);
            return search(key.begin(),key.measure());
        }


        MPI_Datatype data_types:: operator()(const rtti &tid) const
        {
            const MPI_Datatype *ptr = query(tid);
            if(!ptr)
            {
                const string &name = tid.name();
                throw exception("no MPI_Datatype for <%s>", name());
            }
            return *ptr;
        }
    }
}

