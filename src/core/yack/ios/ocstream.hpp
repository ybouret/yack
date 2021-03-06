//! \file

#ifndef YACK_IOS_OCSTREAM_INCLUDED
#define YACK_IOS_OCSTREAM_INCLUDED 1

#include "yack/ios/ostream.hpp"
#include "yack/ios/c/writable.hpp"

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! output C stream
        //
        //______________________________________________________________________
        class ocstream : public ostream
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~ocstream() throw();                                        //!< cleanup
            explicit ocstream(const ios::cstderr_t &);                          //!< link to stderr
            explicit ocstream(const ios::cstdout_t &);                          //!< link to stdout
            explicit ocstream(const char   *filename, const bool append=false); //!< open file
            explicit ocstream(const string &filename, const bool append=false); //!< open file

            //__________________________________________________________________
            //
            // ostream interface
            //__________________________________________________________________
            virtual void   write(const char C);
            virtual void   flush();
            virtual void   frame(const void *,const size_t);
            virtual size_t operator()(const char *fmt,...);

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            static void overwrite(const char   *filename);                  //!< overwrite filename
            static void overwrite(const string &filename);                  //!< overwrite filename
            static void echo(const char   *filename, const char *fmt, ...); //!< append to filename
            static void echo(const string &filename, const char *fmt, ...); //!< append to filename


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ocstream);
            writable_file file;
        };

        //______________________________________________________________________
        //
        //
        //! output APPEND C stream
        //
        //______________________________________________________________________
        class acstream : public ocstream
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~acstream() throw();                //!< cleanup
            explicit acstream(const ios::cstderr_t &);  //!< default stderr
            explicit acstream(const ios::cstdout_t &);  //!< default stdout
            explicit acstream(const char *);            //!< file+append
            explicit acstream(const string &);          //!< file+append

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(acstream);
        };
    }
}

#endif
