
#include "yack/mpi++/data/string.hxx"
#include "yack/mpi++/mpi.hpp"

namespace yack
{
    namespace __mpi
    {

        string_io:: string_io() : self_data_io() { }
        string_io:: ~string_io() noexcept {}

        void string_io:: send1(const mpi    &MPI,
                               const string &str,
                               const int     dst,
                               const int     tag) const
        {
            const size_t len = str.size();
            MPI.Send1(len,dst,tag);
            if(len)
            {
                MPI.Send(str(),len,MPI_BYTE,1,dst,tag);
            }
        }

        void string_io:: recv1(const mpi    &MPI,
                               string       &str,
                               const int     src,
                               const int     tag) const
        {
            const size_t len = MPI.Recv1<size_t>(src,tag);
            if(len>0)
            {
                string tmp(len,as_capacity,true);
                MPI.Recv((void*)tmp(),len,MPI_BYTE,1,src,tag);
                str.xch(tmp);
            }
        }

        void string_io:: send(const mpi   &MPI,
                              const void  *ptr,
                              const size_t num,
                              const int    dst,
                              const int    tag) const
        {
            const string *s = static_cast<const string *>(ptr);
            for(size_t i=num;i>0;--i)
            {
                send1(MPI,*(s++),dst,tag);
            }
        }

        void string_io:: recv(const mpi   &MPI,
                              void        *ptr,
                              const size_t num,
                              const int    src,
                              const int    tag) const
        {
            string *s = static_cast<string *>(ptr);
            for(size_t i=num;i>0;--i)
            {
                recv1(MPI,*(s++),src,tag);
            }
        }
        
    }

}

