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

        class pstream : public ios::istream
        {
        public:
            typedef shared_ptr<int> result;

            virtual ~pstream() throw();

            template <typename COMMAND> inline
            explicit pstream(const COMMAND &cmd, const result &res) :
            handle( init(cmd) ), retval( res ) {}

            void load(sequence<string> &lines);

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
