
//! \file

#ifndef YACK_LOCAL_FS_INCLUDED
#define YACK_LOCAL_FS_INCLUDED 1

#include "yack/fs/vfs.hpp"
#include "yack/singleton.hpp"

namespace yack
{
    //! local file system interface
    class local_fs : public vfs, public singleton<local_fs>
    {
    public:
        static const at_exit::longevity life_time=0; //!< life time
        static const char               call_sign[]; //!< "local_fs"
        

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(local_fs);
        virtual ~local_fs() throw();
        explicit local_fs() throw();
    };


}

#endif
