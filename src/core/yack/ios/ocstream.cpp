


#include "yack/ios/ocstream.hpp"
#include <cstdarg>

namespace yack
{
    namespace ios
    {

        ocstream:: ~ocstream() throw()
        {
            try {
                flush();
            }
            catch(...)
            {

            }
        }


        ocstream:: ocstream(const cstderr_t &_) :
        file(_)
        {

        }

        ocstream:: ocstream(const cstdout_t &_) :
        file(_)
        {
        }

        ocstream:: ocstream(const char *filename, const bool append) :
        file(filename,append)
        {
        }

        ocstream:: ocstream(const string &filename, const bool append) :
        file(filename,append)
        {
        }

        void ocstream:: overwrite(const char *filename)
        {
            volatile ocstream temp(filename,false);
        }

        void ocstream:: write(const char C)
        {
            file.put(C);
        }

        void ocstream:: frame(const void *addr, const size_t size)
        {
            file.put(addr,size);
        }

        void ocstream:: flush()
        {
            file.flush();
        }


        size_t ocstream:: operator()(const char *fmt,...)
        {
            assert(NULL!=fmt);
            va_list ap;
            va_start(ap,fmt);
            const size_t nw = file.put(fmt,&ap);
            va_end(ap);
            return nw;
        }

        void ocstream:: echo(const char *filename, const char *fmt, ...)
        {
            assert(NULL!=filename);
            assert(NULL!=fmt);
            ocstream temp(filename,true);

            va_list ap;
            va_start(ap,fmt);
            temp.file.put(fmt,&ap);
            va_end(ap);
        }


    }

}
