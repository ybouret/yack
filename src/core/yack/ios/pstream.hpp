//! \file

#ifndef YACK_IOS_PSTREAM_INCLUDED
#define YACK_IOS_PSTREAM_INCLUDED 1


#include "yack/ios/istream.hpp"
#include "yack/ptr/shared.hpp"
#include "yack/container/sequence.hpp"

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! high-level process stream
        //
        //______________________________________________________________________
        class pstream : public ios::istream
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef shared_ptr<int> result; //!< persistent result

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~pstream() throw(); //!< set result

            //! open a command, register result
            template <typename COMMAND> inline
            explicit pstream(const COMMAND &cmd, const result &res) :
            handle( init(cmd) ), retval( res ) {}

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! append all parsed lines
            void load(sequence<string> &lines);

            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________

            //! transform command into output lines
            template <typename COMMAND> static inline
            void fill(sequence<string> &lines, const COMMAND &cmd)
            {
                result  res = new int;
                {
                    pstream inp(cmd,res);
                    inp.load(lines);
                }
                check(res,cmd);
            }


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(pstream);
            void   *handle;
            result  retval;

            static void *init(const string &cmd);
            static void *init(const char   *cmd);
            static void  check(const result &, const string &cmd);
            static void  check(const result &, const char   *cmd);

            virtual bool   query_(char &C);
            virtual size_t fetch_(void *addr, const size_t size);

        };
    }
}

#endif
