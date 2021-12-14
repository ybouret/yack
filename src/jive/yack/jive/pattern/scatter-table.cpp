#include "yack/jive/pattern/scatter-table.hpp"


namespace yack
{
    namespace jive
    {

        scatter:: node:: node(const pattern &p, const void *h) throw() :
        object(),
        next(0),
        host(p),
        data(h)
        {}

        scatter:: node:: node(const node &other) throw() :
        object(),
        next(0),
        host(other.host),
        data(other.data)
        {}


        scatter:: node:: ~node() throw() {}


    }

}



namespace yack
{
    namespace jive
    {
        scatter:: hasher::  hasher() throw() {}
        scatter:: hasher:: ~hasher() throw() {}

        size_t scatter::hasher:: operator()(const uint8_t &code) const throw()
        {
            return code;
        }
    }

}

#include "yack/jive/pattern/first-bytes.hpp"
#include "yack/jive/pattern.hpp"
#include "yack/exception.hpp"
#include "yack/ios/ascii/hybrid.hpp"
#include <iomanip>

namespace yack
{
    namespace jive
    {
        scatter:: table:: table() throw() : table_() {}

        scatter:: table:: ~table() throw() {}

        void scatter::table:: operator()(const pattern &p, const void *h)
        {
            first_bytes fc;
            p.firsts(fc);
            for(const domain *dom=fc.head;dom;dom=dom->next)
            {
                const size_t up = dom->upper;
                for(size_t code=dom->lower;code<=up;++code)
                {
                    use(code,p,h);
                }
            }
        }

        void scatter::table:: use(const uint8_t code, const pattern &p, const void *h)
        {
            static const char fn[] = "jive::scatter:table: ";

            const slot *s = search(code);
            if(!s)
            {
                const slot empty;
                if( !insert(code,empty)  ) throw exception("%s unexpected insert failure",fn);
                if(NULL==(s=search(code))) throw exception("%s unexpected search failure",fn);
            }
            assert(NULL!=s);
            coerce(*s).stash( new node(p,h) );

        }

        const scatter::node * scatter::table:: operator[](const uint8_t code) const throw()
        {
            const slot *s = search(code);
            return (NULL!=s) ? s->head : NULL;
        }

        std::ostream & operator<<(std::ostream &os, const scatter::table &lut)
        {
            os << "<scatter::table size='" << lut.size() << "'>" << std::endl;
            for(scatter::table::const_iterator it=lut.begin();it!=lut.end();++it)
            {
                const uint8_t        code = it->key();
                const scatter::slot &data = *it;
                os << "\t@" << std::setw(3) << ios::ascii::hybrid[code] << " :";
                for(const scatter::node *info=data.head;info;info=info->next)
                {
                    os << ' ' << info->host.express();
                }
                os << std::endl;
            }
            os << "<scatter::table/>";
            return os;
        }


    }

}


