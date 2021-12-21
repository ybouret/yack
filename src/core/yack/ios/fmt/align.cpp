#include "yack/ios/fmt/align.hpp"
#include <cstring>

namespace yack
{

    namespace ios
    {
        align:: ~align() throw()
        {

        }

        align:: align(const string &s, const size_t w, const justify j) throw() :
        entry( s() ),
        count( s.size() ),
        width(w),
        jflag(j)
        {
        }

        align:: align(const char *text, const size_t w, const justify j) throw() :
        entry(text),
        count(text?strlen(text):0),
        width(w),
        jflag(j)
        {

        }


        align:: align(const align &a) throw() :
        entry(a.entry),
        count(a.count),
        width(a.width),
        jflag(a.jflag)
        {
        }

        void  align:: out(std::ostream &os) const
        {
            for(size_t i=0;i<count;++i) os << entry[i];
        }

        void  align:: spc(std::ostream &os, size_t n)
        {
            while(n-- > 0) os << ' ';
        }


        std::ostream &align:: display(std::ostream &os) const
        {
            if(count>=width)
            {
                out(os);
            }
            else
            {
                const size_t space = width - count;
                switch (jflag)
                {
                    case left:  out(os); spc(os,space); break;
                    case right: spc(os,space); out(os); break;
                    case center: {
                        const size_t half = space>>1;
                        spc(os,half); out(os); spc(os,space-half);
                    } break;
                }
            }
            return os;
        }

        std::ostream & operator<<(std::ostream &os, const align &a)
        {
            return a.display(os);
        }



    }

}
