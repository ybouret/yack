
//! \file

#ifndef YACK_VFS_INCLUDED
#define YACK_VFS_INCLUDED 1

#include "yack/string.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/ptr/arc.hpp"

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
        typedef arc_ptr<const string> string_ptr; //!< alias

        //______________________________________________________________________
        //
        //! a file system entry
        //______________________________________________________________________
        class entry : public object
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef unsigned    attr_t;                          //!< alias
            static const attr_t attr_reg = 0x01;                 //!< regular file
            static const attr_t attr_dir = 0x02;                 //!< directory
            static const attr_t attr_lnk = 0x04;                 //!< link
            static const attr_t attr_unk = 0x00;                 //!< unknown
            static const attr_t attr_msk = attr_reg|attr_dir;    //!< helper

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit entry(const vfs     &root,
                           const string  &here);        //!< setup with full_path
            virtual ~entry() throw();                   //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool is_unk() const throw(); //!< check if unknown
            bool is_reg() const throw(); //!< check if regular
            bool is_dir() const throw(); //!< check if directory
            bool is_lnk() const throw(); //!< check if link

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            entry             *next;                    //!< for list/pool
            entry             *prev;                    //!< for list/pool
            const string_ptr   path;                    //!< full path
            const char * const base;                    //!< base name
            const char * const cext;                    //!< extension
            const attr_t       attr;                    //!< attributes

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
            const vfs     &root;       //!< root fs
            const string   path;       //!< open path

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual       ~scanner() throw(); //!< cleanup
        protected:
            explicit       scanner(const vfs    &fs,
                                   const string &dirname); //!< open folder

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(scanner);
        };



        //______________________________________________________________________
        //
        // interface
        //______________________________________________________________________
        virtual void          remove_file(const string &path)         = 0; //!< remove given filename
        virtual scanner      *open_folder(const string &path)         = 0; //!< get a new scanning object
        virtual entry::attr_t get_attr_of(const string &path)   const = 0; //!< attribute
        virtual void          make_folder(const string &dirname,bool) = 0; //!< 

        //______________________________________________________________________
        //
        // non-virtual interface: remove file(s)
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
        // non-virtual interface: create directory
        //______________________________________________________________________


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
