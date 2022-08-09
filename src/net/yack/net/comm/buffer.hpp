//! \file

#ifndef YACK_NET_COMM_BUFFER_INCLUDED
#define YACK_NET_COMM_BUFFER_INCLUDED 1


#include "yack/object.hpp"
#include "yack/net/channel.hpp"

namespace yack
{
    namespace net
    {

        namespace comm
        {
            class buffer : public object
            {
            public:
                static const size_t min_size = 128;

                explicit buffer(const size_t n);
                virtual ~buffer() throw();
                
                buffer *next;
                buffer *prev;

                void   restart() throw();
                size_t written() const throw() { return wpos-rpos; }
                size_t vacuous() const throw() { return last-wpos; }

                bool   recv(channel &input, const int flags)
                {
                    assert(vacuous()>0);
                    const size_t nr = input.recv(wpos,vacuous(),flags);
                    if(nr)
                    {
                        assert(nr<=vacuous());
                        wpos += nr;
                        assert(wpos<=last);
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }



            private:
                YACK_DISABLE_COPY_AND_ASSIGN(buffer);
                const size_t          bytes;
                uint8_t * const       entry;
                const uint8_t * const last;
                uint8_t              *rpos;
                uint8_t              *wpos;


            };
        }

    }
}


#endif

