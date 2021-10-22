

//! \file
#ifndef YACK_IOS_C_FILE_INCLUDED
#define YACK_IOS_C_FILE_INCLUDED 1

#include "yack/system/error.hpp"
#include "yack/setup.hpp"

namespace yack
{
    namespace ios
    {
        typedef int2type<0>   cstdin_t;   //!< alias  for stdin
        extern const cstdin_t cstdin;     //!< helper for stdin

        typedef int2type<1>    cstdout_t; //!< alias  for stdout
        extern const cstdout_t cstdout;   //!< helper for stdout

        typedef int2type<2>    cstderr_t; //!< alias  for stderr
        extern const cstderr_t cstderr;   //!< helper for stderr

        //______________________________________________________________________
        //
        //
        //! legacy C FILE operations
        //
        //______________________________________________________________________
        class c_file
        {
        public:
            //__________________________________________________________________
            //
            // types and definitiosn
            //__________________________________________________________________
            //! opening mode
            enum mode_type
            {
                r, //!< "rb"
                w, //!< "wb"
                a  //!< "ab"
            };
            static system_error_t last_close_error; //!< if necessary



            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void flush();

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~c_file() throw();                //!< cleanup
        protected:
            explicit c_file(const cstdin_t  &);       //!< link to stdin
            explicit c_file(const cstdout_t &);       //!< link to stdout
            explicit c_file(const cstderr_t &);       //!< link to stderr
            explicit c_file(const char *, mode_type); //!< open a file
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            void      *handle; //!< anonymous FILE

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(c_file);
            const  bool   _close;
            static void * _open(const char *, mode_type);
        };

    }

}

#endif
