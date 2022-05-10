
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
    class localFS : public vfs, public singleton<localFS>
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
        virtual void          remove_file(const string &path);         //!< unlink/DeleteFile
        virtual scanner      *open_folder(const string &path);         //!< use opendir/FindFirstFile
        virtual entry::attr_t get_attr_of(const string &path)   const; //!< from lstat/GetFileAttributes
        virtual void          make_folder(const string &dirname);      //!< from mkdir/CreateDirectory
        virtual uint64_t      query_bytes(const string &path)   const; //!< query bytes
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(localFS);
        virtual ~localFS() throw();
        explicit localFS() throw();
        friend class singleton<localFS>;
    };


}

#endif
