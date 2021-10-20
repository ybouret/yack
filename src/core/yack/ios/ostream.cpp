

#include "yack/ios/ostream.hpp"
#include "yack/exception.hpp"
namespace yack
{
    namespace ios
    {

        ostream:: ~ostream() throw()
        {
        }


        ostream:: ostream() throw()
        {
        }

        void ostream:: frame(const char *msg)
        {
            if(msg)
            {
                while(*msg) write( *(msg++) );
            }
        }

        ostream & ostream:: operator<<(const char C)
        {
            write(C);
            return *this;
        }

        ostream & ostream:: operator<<(const char *msg)
        {
            frame(msg);
            return *this;
        }

        void ostream:: operator()(const char * , ...)
        {
            throw exception("not implemented");
        }
    }

}
