#include "yack/jive/tags.hpp"
#include "yack/exception.hpp"
#include <cstring>

namespace yack
{
    namespace jive
    {

        const char tags::call_sign[] = "jive::tags";

        tags:: ~tags() throw()
        {
        }

        tags:: tags() throw() : singleton<tags>(), tags_()
        {
        }

        string * tags::make(const char *buffer, const size_t buflen)
        {
            static const char fn[] =  "::make";
            if(NULL==buffer) throw exception("%s%s(NULL data)", call_sign,fn);
            if(buflen<1)     throw exception("%s%s(empty data)",call_sign,fn);
            static tags &self = instance();
            YACK_LOCK(self.access);
            tag *p = self.tree.search(buffer,buflen);
            if(p)
            {
                // registered
                return & **p;
            }
            else
            {
                // must create
                string    *ans = new string(buffer,buflen);
                const  tag tid(ans);
                if( !self.insert(tid) )
                    throw exception("%s%s(unexpected failure to insert '%s')", call_sign, fn, (*ans)() );
                return ans;
            }
        }

        string * tags::make(const char *buffer)
        {
            return make(buffer,buffer?strlen(buffer):0);
        }


        string * tags::make(const char c)
        {
            return make(&c,1);
        }

        string * tags::make(const string &s)
        {
            return make(s(),s.size());
        }

        string * tags::make(const tag &t)
        {
            static tags &self = instance();
            YACK_LOCK(self.access);
            const tag   *p = self.search(*t);
            if(p)
                return & *coerce(*p);
            else
                return make(*t);
        }

        string *tags::make(string *h)
        {
            assert(h);
            const  tag t =  h;
            return make(t);
        }

        void tags:: gv()
        {
            static const char dot_file[] ="jive-tags.dot";
            static tags &self = instance();
            YACK_LOCK(self.access);
            self.tree.gv(dot_file);
            ios::vizible::render(dot_file);
        }



    }
}
