
#include "yack/jive/module.hpp"
#include "yack/ios/icstream.hpp"
#include "yack/ios/imstream.hpp"

namespace yack
{
    namespace jive
    {
        module:: ~module() throw()
        {
        }
        
        module:: module(const string &filename) :
        context(filename),
        handle( new ios::icstream(filename) ),
        origin(from_file)
        {
        }
        
        module:: module(const char *filename) :
        context(filename),
        handle( new ios::icstream(filename) ),
        origin(from_file)
        {
        }

        module:: module() :
        context(YACK_STDIN),
        handle( new ios::icstream(ios::cstdin) ),
        origin(from_file)
        {
        }
      
        module *module:: open_stdin()
        {
            return new module();
        }

        module:: module(const string &dataname, const void *addr, const size_t size) :
        context(dataname),
        handle( new ios::imstream(addr,size) ),
        origin( from_data )
        {}
        
        module:: module(const char *dataname, const void *addr, const size_t size) :
        context(dataname),
        handle( new ios::imstream(addr,size) ),
        origin( from_data )
        {}


        character *module:: get()
        {
            char C = 0;
            if(handle->query(C))
            {
                character *ch = new character(*this,C);
                ++coerce(column);
                return ch;
            }
            else
            {
                return NULL;
            }
        }
        
        void module:: newline() throw()
        {
            ++coerce(line);
            coerce(column) = 1;
        }

        bool module:: gets(token &line)
        {
            line.release();
            ios::characters temp;
            if(handle->gets(temp))
            {
                while(temp.size)
                {
                    const ios::character &ch = *temp.head;
                    ++coerce(column);
                    line.push_back( new character(*this,*ch) );
                    delete temp.pop_front();
                }
                newline();
                return true;
            }
            else
            {
                return false;
            }
        }

        
    }
}

