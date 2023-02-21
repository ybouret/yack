#include "yack/jive/pattern/scatter-table.hpp"


namespace yack
{
    namespace jive
    {

        scatter:: node:: node(const void *h) noexcept :
        object(),
        next(0),
        data(h)
        {}

        scatter:: node:: node(const node &other) noexcept :
        object(),
        next(0),
        data(other.data)
        {}


        scatter:: node:: ~node() noexcept {}

        const void * scatter:: node:: operator*() const noexcept
        {
            return data;
        }

    }

}



namespace yack
{
    namespace jive
    {
        scatter:: hasher::  hasher() noexcept {}
        scatter:: hasher:: ~hasher() noexcept {}

        size_t scatter::hasher:: operator()(const uint8_t &code) const noexcept
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
        scatter:: table:: table() noexcept : table_() {}

        scatter:: table:: ~table() noexcept {}

        void scatter::table:: store(const pattern &p, const void *h)
        {
            first_bytes fc;
            p.firsts(fc);
            for(const domain *dom=fc.head;dom;dom=dom->next)
            {
                const size_t up = dom->upper;
                for(size_t code=dom->lower;code<=up;++code)
                {
                    use(code,h);
                }
            }
        }

        void scatter::table:: use(const uint8_t code, const void *h)
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
            coerce(*s).stash( new node(h) );

        }

        const scatter::node * scatter::table:: query(const uint8_t code, size_t &count) const noexcept
        {
            const slot *s = search(code);
            if(s)
            {
                count = s->size;
                return  s->head;
            }
            else
            {
                count = 0;
                return NULL;
            }
        }

        std::ostream & operator<<(std::ostream &os, const scatter::table &lut)
        {
            os << "<scatter::table size='" << lut.size() << "'>" << std::endl;
            size_t idx = 0;
            for(scatter::table::const_iterator it=lut.begin();it!=lut.end();++it)
            {
                const uint8_t        code = it->key();
                const scatter::slot &data = *it;
                os << ' ' << std::setw(3) << ios::ascii::hybrid[code];
                os << ':' << std::setw(2) << data.size;
                if( 0 == (++idx%16) ) os << std::endl;
            }
            os << std::endl;
            os << "<scatter::table/>";
            return os;
        }


    }

}


