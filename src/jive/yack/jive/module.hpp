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
        class module : public context
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
            // C++
            //__________________________________________________________________
            virtual        ~module() throw(); //!< cleanup
            static module *open_stdin();      //!< open stdin

            //! open a C file by its name
            template <typename FILENAME> inline
            static module *open_file(const FILENAME &filename)
            {
                static const int2type<from_file> as_file = {};
                return new module(filename,as_file);
            }
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            character *get();             //!< get next char, NULL => EOF
            void       newline() throw(); //!< update context
            bool       gets(token &line); //!< helper to get line
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            input        handle; //!< used handle
            const whence origin; //!< keep trace of origin


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(module);
            module(const string &filename, const int2type<from_file> &);
            module(const char   *filename, const int2type<from_file> &);
            module(const int2type<from_file> &);
            
        };
    }
}

#endif

