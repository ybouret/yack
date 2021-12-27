#include "yack/string/cxx-name.hpp"

namespace yack
{

    static inline string cxx_(string &res)
    {
        for(size_t i=res.size();i>0;--i)
        {
            char &c = res[i];
            if(c>='a'&&c<='z') continue;
            if(c>='A'&&c<='Z') continue;
            if(c>='0'&&c<='9') continue;
            if(c=='.')         continue;
            c = '_';
        }
        return res;
    }

    string cxx_name::of(const string &src)
    {
        string res(src);
        return cxx_(res);
    }


    string cxx_name:: of(const char *src)
    {
        string res(src);
        return cxx_(res);
    }

}
