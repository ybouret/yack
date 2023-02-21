//! \file

#ifndef YACK_JIVE_MODULE_INCLUDED
#define YACK_JIVE_MODULE_INCLUDED 1

#include "yack/jive/token.hpp"
#include "yack/ios/istream.hpp"

namespace yack
{
    namespace jive
    {
        //______________________________________________________________________
        //
        //
        //! alias for input
        //
        //______________________________________________________________________
        typedef arc_ptr<ios::istream> input;
        
        //______________________________________________________________________
        //
        //
        //! a module transform a source of chars intro a source of jive::chars
        //
        //______________________________________________________________________
        class module : public object, public context
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            //! kind of module
            enum whence
            {
                from_file, //!< loaded from file
                from_data  //!< loaded from data
            };
            

            //__________________________________________________________________
            //
            // opening files
            //__________________________________________________________________
            static module *open_stdin();      //!< open stdin

            //! open a C file by its name
            template <typename FILENAME> inline
            static module *open_file(const FILENAME &filename)
            {
                return new module(filename);
            }

            //__________________________________________________________________
            //
            // opening data
            //__________________________________________________________________

            //! open persistent data
            template <typename DATANAME> inline
            static module *open_data(const DATANAME &dataname, const void *addr, const size_t size)
            {
                assert(yack_good(addr,size));
                return new module(dataname,addr,size);
            }

            //! open persistent buffer
            template <typename DATANAME> inline
            static module *open_data(const DATANAME &dataname, const memory::ro_buffer &buff)
            {
                return new module(dataname,buff.ro_addr(),buff.measure());
            }

            //! open persistent text
            template <typename DATANAME> inline
            static module *open_data(const DATANAME &dataname, const char *text)
            {
                return new module(dataname,text,text?strlen(text):0);
            }

            //! helper when data=dataname...
            template <typename DATANAME> inline
            static module *open_data(const DATANAME &dataname)
            {
                return open_data(dataname,dataname);
            }


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            character *get();             //!< get next char, NULL => EOF
            void       newline() noexcept; //!< update context
            bool       gets(token &line); //!< helper to get line
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
        private: input        handle; //!< used handle
        public:  const whence origin; //!< keep trace of origin

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual        ~module() noexcept; //!< cleanup
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(module);
            module(const string &filename);
            module(const char   *filename);
            module(); //!< stdin

            module(const string &dataname, const void *addr, const size_t size);
            module(const char   *dataname, const void *addr, const size_t size);

            
        };
    }
}

#endif

