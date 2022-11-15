
#include "yack/chem/library.hpp"
#include "yack/system/exception.hpp"
#include "yack/string/tokenizer.hpp"
#include "yack/string/ops.hpp"
#include "yack/randomized/bits.hpp"
#include <iomanip>
#include <cmath>

namespace yack
{
    namespace chemical
    {

        double library:: concentration(randomized::bits &ran) throw()
        {
            static const double lower = min_exp10;
            static const double ampli = max_exp10-min_exp10;
            const double p = lower + ampli * ran.to<double>();
            return pow(10.0,p);
        }

        void library:: fill(writable<double> &C, randomized::bits &ran) const
        {
            for(const snode *node=head();node;node=node->next)
            {
                const species &s = ***node;
                const double   c = concentration(ran);
                if(s.rank>0) C[*s] = c; else C[*s] = -c;
            }
        }



        const char library:: clid[] = "chemical::library";

        library:: ~library() throw()
        {
        }

        library:: library() throw() : gathering(), readable<const species>(), sdb()
        {
        }

        library:: library(const library &other) : collection(), gathering(), readable<const species>(), sdb()
        {

            for(const snode *node=other.head();node;node=node->next)
            {
                use( new species(***node) );
            }

            assert(maxlen==other.maxlen);

        }



        const snode * library:: head() const throw()
        {
            return (*sdb.tree).head;
        }

        library::const_type & library:: operator[](const size_t i) const throw()
        {
           return *** (*sdb.tree).get(i);
        }

        size_t library:: size() const throw()
        {
            return (*sdb.tree).size;
        }


        const species & library:: use(species *s)
        {
            assert(s);
            const species::pointer sp(s);assert(**s==size()+1);
            if(latched())       throw imported::exception(clid,"is LOCKED");
            if(!sdb.insert(sp)) throw imported::exception(clid,"multiple species '%s'", s->name() );
            absorb(*sp);
            return *sp;
        }

        std::ostream & operator<<(std::ostream  &os,
                                  const library &lib)
        {
            os << '{' << std::endl;
            for(const snode *node=lib.head();node;node=node->next)
            {
                const species &s = ***node;
                lib.pad(os << '[' << s.name <<']',s);
                os << " | # "    << std::setw(5) << *s;
                os << " | z="    << std::setw(5) << s.z;
                os << " | rank=" << std::setw(5) << s.rank;
                os << std::endl;
            }
            os << '}';
            return os;
        }


        const species * library:: look_up(const string &uid) const throw()
        {
            const species::pointer *pps = sdb.search(uid);
            return pps ? & **pps : NULL;
        }

        const species & library:: operator[](const string &uid) const
        {
            const species::pointer *pps = sdb.search(uid);
            if(!pps) throw imported::exception(clid,"no '%s'",uid());
            return **pps;
        }

        const species & library:: operator[](const char *uid) const
        {
            const string _(uid);
            return (*this)[_];
        }


        library & library:: operator<<(const string &input)
        {
            library       &self = *this;
            vector<string> fields(8,as_capacity);
            tokenizer::split_with(':', fields, input);
            const size_t n = fields.size();
            for(size_t i=1;i<=n;++i)
            {
                string &field = fields[i];
                strops::strip_with(" \t", 2, field);
                (void)self(field);
            }
            return *this;
        }

        library & library:: operator<<(const char *input)
        {
            const string _(input);
            return (*this) << _;
        }

        std::ostream & library:: frame(std::ostream &os, const readable<double> &C) const
        {
            for(const snode *node = head();node;node=node->next)
            {
               (void)  (***node).frame(os << ' ',C);
            }
            return os;
        }

        ios::ostream & library:: frame(ios::ostream &os, const readable<double> &C) const
        {
            for(const snode *node = head();node;node=node->next)
            {
                (void)  (***node).frame(os << ' ',C);
            }
            return os;
        }

    }

}
