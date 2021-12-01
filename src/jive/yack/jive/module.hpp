//! \file

#ifndef YACK_JIVE_MODULE_INCLUDED
#define YACK_JIVE_MODULE_INCLUDED 1

#include "yack/jive/token.hpp"
#include "yack/ios/istream.hpp"

namespace yack
{
    namespace jive
    {
        typedef arc_ptr<ios::istream> input;
        
        class module : public context
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            enum whence
            {
                from_file
            };
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~module() throw();
            
            template <typename FILENAME> inline
            static module *open_file(const FILENAME &filename)
            {
                static const int2type<from_file> as_file = {};
                return new module(filename,as_file);
            }
            static module *open_stdin();
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            character *get();
            void       newline() throw();
            bool       gets(token &line);
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            input        handle;
            const whence origin;


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(module);
            module(const string &filename, const int2type<from_file> &);
            module(const char   *filename, const int2type<from_file> &);
            module(const int2type<from_file> &);
            
        };
    }
}

#endif

