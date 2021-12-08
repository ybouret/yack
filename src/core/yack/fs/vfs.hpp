
//! \file

#ifndef YACK_VFS_INCLUDED
#define YACK_VFS_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/string/fwd.hpp"

namespace yack
{

    //! Virtual File System interface
    class vfs
    {
    public:
        static const char *get_base_name(const char *path, const size_t plen) throw(); //!< get base name
        static const char *get_base_name(const char *path) throw();                    //!< get base name
        static const char *get_base_name(const string &path) throw();                  //!< get base name


        static const char *get_extension(const char   *path, const size_t plen) throw(); //!< get extension, NULL if none
        static const char *get_extension(const char   *path)                    throw(); //!< get extension
        static const char *get_extension(const string &path) throw();                    //!< get extension
        
        static string      make_null_ext(const char   *path, const size_t plen); //!< remove extension
        static string      make_null_ext(const char   *path);                    //!< remove extension
        static string      make_null_ext(const string &path);                    //!< remove extension

        static string      new_extension(const char   *path, const size_t plen, const char *ext); //!< change extension
        static string      new_extension(const char   *path, const char   *ext);                  //!< change extension
        static string      new_extension(const string &path, const char   *ext);                  //!< change extension
        static string      new_extension(const string &path, const string &ext);                  //!< change extension


        virtual ~vfs() throw();
    protected:
        explicit vfs() throw();
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(vfs);
    };



}

#endif
