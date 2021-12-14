
//! \file

#ifndef YACK_LOCAL_FS_INCLUDED
#define YACK_LOCAL_FS_INCLUDED 1

#include "yack/fs/vfs.hpp"
#include "yack/singleton.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! local file system interface
    //
    //__________________________________________________________________________
    class local_fs : public vfs, public singleton<local_fs>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        static const at_exit::longevity life_time=1000; //!< life time
        static const char               call_sign[];    //!< "local_fs"

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        virtual void          remove_file(const string &path);       //!< unlink/DeleteFile
        virtual scanner      *open_folder(const string &path);       //!< use opendir/FindFirstFile
        virtual entry::attr_t get_attr_of(const string &path) const;

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(local_fs);
        virtual ~local_fs() throw();
        explicit local_fs() throw();
        friend class singleton<local_fs>;
    };


}

#endif
