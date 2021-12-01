
#include "yack/jive/module.hpp"
#include "yack/ios/icstream.hpp"

namespace yack
{
    namespace jive
    {
        module:: ~module() throw()
        {
        }
        
        module:: module(const string &filename,
                        const int2type<from_file> &) :
        context(filename),
        handle( new ios::icstream(filename) ),
        origin(from_file)
        {
        }
        
        module:: module(const char *filename,
                        const int2type<from_file> &) :
        context(filename),
        handle( new ios::icstream(filename) ),
        origin(from_file)
        {
        }

        module:: module(const int2type<from_file> &) :
        context(YACK_STDIN),
        handle( new ios::icstream(ios::cstdin) ),
        origin(from_file)
        {
        }
      
        module *module:: open_stdin()
        {
            static const int2type<from_file> use_stdin = {};
            return new module(use_stdin);
        }
        
        character *module:: get()
        {
            char C = 0;
            if(handle->query(C))
            {
                ++coerce(column);
                return new character(*this,C);
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

