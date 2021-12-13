
//! \file

#ifndef YACK_VFS_INCLUDED
#define YACK_VFS_INCLUDED 1

#include "yack/string.hpp"
#include "yack/data/list/cxx.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! Virtual File System interface
    //
    //__________________________________________________________________________
    class vfs
    {
    public:
        //______________________________________________________________________
        //
        // path utilities
        //______________________________________________________________________
        static const char *get_base_name(const char *path, const size_t plen) throw(); //!< get base name
        static const char *get_base_name(const char *path) throw();                    //!< get base name
        static const char *get_base_name(const string &path) throw();                  //!< get base name
        static void        make_dir_name(string &path);                                //!< ensure path is a directory
        static string      get_dir_name(const char   *path);                           //!< get directory name
        static string      get_dir_name(const string &path);                           //!< get directory name

        static const char *get_extension(const char   *path, const size_t plen) throw(); //!< get extension, NULL if none, use BASENAME!
        static const char *get_extension(const char   *path)                    throw(); //!< get extension
        static const char *get_extension(const string &path) throw();                    //!< get extension
        
        static string      make_null_ext(const char   *path, const size_t plen); //!< remove extension
        static string      make_null_ext(const char   *path);                    //!< remove extension
        static string      make_null_ext(const string &path);                    //!< remove extension

        static string      new_extension(const char   *path, const size_t plen, const char *ext); //!< change extension
        static string      new_extension(const char   *path, const char   *ext);                  //!< change extension
        static string      new_extension(const string &path, const char   *ext);                  //!< change extension
        static string      new_extension(const string &path, const string &ext);                  //!< change extension

        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________

        //______________________________________________________________________
        //
        //! a file system entry
        //______________________________________________________________________
        class entry : public object
        {
        public:
            virtual ~entry() throw();       //!< cleanup
            explicit entry(const string &); //!< setup with full_path
            entry *next;             //!< for list/pool
            entry *prev;             //!< for list/pool
            const string       path; //!< full path
            const char * const base; //!< base name
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(entry);
        };

        //______________________________________________________________________
        //
        //! alias
        //______________________________________________________________________
        typedef cxx_list_of<entry> entries;

        //______________________________________________________________________
        //
        //! file system scanner API
        //______________________________________________________________________
        class scanner : public object
        {
        public:
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            virtual entry *next() = 0; //!< get next entry

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string   path;       //!< open path

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual       ~scanner() throw(); //!< cleanup
        protected:
            explicit       scanner(const string &dirname); //!< open folder

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(scanner);
        };

        //______________________________________________________________________
        //
        // interface
        //______________________________________________________________________
        virtual void     remove_file(const string &path) = 0; //!< remove given filename
        virtual scanner *open_folder(const string &path) = 0; //!< get a new scanning object

        //______________________________________________________________________
        //
        // non-virtual interface
        //______________________________________________________________________
        void remove_file(const char *path); //!< wrapper

        //! helper, a.k.a rm -f
        template <typename PATH> inline
        bool try_remove_file(const PATH &path) throw()
        {
            try { remove_file(path); return true; }
            catch(...) { return false; }
        }


        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~vfs() throw();
    protected:
        explicit vfs() throw();
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(vfs);
    };



}

#endif
