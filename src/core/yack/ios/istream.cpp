

#include "yack/ios/istream.hpp"

namespace yack
{
    namespace ios
    {

        istream:: ~istream() noexcept
        {
        }


        istream:: istream() noexcept : cache()
        {
        }

        bool istream:: query(char &C)
        {
            if(cache.size)
            {
                C = cache.pull_front();
                return true;
            }
            else
            {
                return query_(C);
            }
        }


        void istream:: store(char  C)
        {
            cache.append_front(C);
        }

        size_t istream:: fetch(void *addr, const size_t size)
        {
            if(size)
            {
                assert(NULL!=addr);
                char *ptr = static_cast<char *>(addr);
                if(size<=cache.size)
                {
                    for(size_t i=size;i>0;--i)
                    {
                        *(ptr++) = cache.pull_front();
                    }
                    return size;
                }
                else
                {
                    assert(size>cache.size);
                    const size_t nc = cache.size;
                    while(cache.size)
                    {
                        *(ptr++) = cache.pull_front();
                    }
                    return nc + fetch_(ptr,size-nc);
                }
            }
            else
            {
                return 0;
            }
        }

        bool istream:: gets(characters &line)
        {
            line.release();

            char C = 0;

            while( query(C) )
            {
                switch(C)
                {
                    case '\n':
                        return true;

                    case '\r':
                        if(query(C))
                        {
                            if(C=='\n')
                            {
                                return true;
                            }
                            else
                            {
                                store(C);
                                return true;
                            }
                        }
                        else
                        {
                            // end of file
                            return true;
                        }

                    default:
                        line << C;
                }
            }

            // end of file
            return line.size>0;
        }


    }

}
