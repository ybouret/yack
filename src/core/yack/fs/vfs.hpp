
//! \file

#ifndef YACK_VFS_INCLUDED
#define YACK_VFS_INCLUDED 1

#include "yack/string.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/ptr/arc.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/proto-ostream.hpp"

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
        static const char *get_base_name(const char *path, const size_t plen) noexcept; //!< get base name
        static const char *get_base_name(const char *path) noexcept;                    //!< get base name
        static const char *get_base_name(const string &path) noexcept;                  //!< get base name
        static void        make_dir_name(string &path);                                //!< ensure path is a directory
        static string      get_dir_name(const char   *path);                           //!< get directory name
        static string      get_dir_name(const string &path);                           //!< get directory name

        static const char *get_extension(const char   *path, const size_t plen) noexcept; //!< get extension, NULL if none, use BASENAME!
        static const char *get_extension(const char   *path)                    noexcept; //!< get extension
        static const char *get_extension(const string &path) noexcept;                    //!< get extension
        
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
            virtual ~entry() noexcept;                   //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool is_unk() const noexcept;              //!< check if unknown
            bool is_reg() const noexcept;              //!< check if regular
            bool is_dir() const noexcept;              //!< check if directory
            bool is_lnk() const noexcept;              //!< check if link
            YACK_PROTO_OSTREAM(entry);                //!< display
            const string & operator*() const noexcept; //!< get path content
            
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
            virtual       ~scanner() noexcept; //!< cleanup
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
        virtual void          make_folder(const string &dirname)      = 0; //!< make a new folder
        virtual uint64_t      query_bytes(const string &path)   const = 0; //!< used bytes for path

        //______________________________________________________________________
        //
        // non-virtual interface: testing attributes
        //______________________________________________________________________
        bool is_reg(const string &path) const; //!< entry::attr_reg == get_attr_of(path)
        bool is_reg(const char   *path) const; //!< entry::attr_reg == get_attr_of(path)
        bool is_dir(const string &path) const; //!< entry::attr_dir == get_attr_of(path)
        bool is_dir(const char   *path) const; //!< entry::attr_dir == get_attr_of(path)

        //______________________________________________________________________
        //
        // non-virtual interface: remove file(s)
        //______________________________________________________________________
        void     remove_file(const char *path); //!< wrapper
        scanner *open_folder(const char *path); //!< wrapper
        

        //! helper, a.k.a rm -f
        template <typename PATH> inline
        bool try_remove_file(const PATH &path) noexcept
        {
            try { remove_file(path); return true; }
            catch(...) { return false; }
        }

        //______________________________________________________________________
        //
        // non-virtual interface: create directory
        //______________________________________________________________________
        void create_subdirectory(const string &path); //!< mkdir -p path
        void create_subdirectory(const char   *path); //!< mkdir -p path, alias


        //______________________________________________________________________
        //
        // non-virtual interface: query entries
        //______________________________________________________________________

        //! store entries with matching paths
        /**
         \param elist list of entries to fill
         \param path  path to scan
         \param proc  return true if base_name is to be kept
         */
        template <typename PATH, typename PROC> inline
        void query(entries &elist, PATH &path, PROC &proc)
        {
            elist.release();
            auto_ptr<scanner> scan = open_folder(path);
            entry            *ep   = 0;
            while(NULL!=(ep=scan->next()))
            {
                auto_ptr<entry> eap = ep;
                if(!eap->is_reg())   continue;
                if(!proc(eap->base)) continue;
                elist.push_back(eap.yield());
            }
        }



        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~vfs() noexcept;
    protected:
        explicit vfs() noexcept;
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(vfs);
    };



}

#endif
