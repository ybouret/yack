
#include "yack/chem/library.hpp"
#include "yack/exception.hpp"
#include "yack/chem/builder.hpp"
#include "yack/randomized/bits.hpp"
#include "yack/string/tokenizer.hpp"

namespace yack
{

    namespace chemical
    {

        library:: ~library() throw() {}

        library:: library() : compendium(), db() 
        {

        }

        size_t library:: size() const throw()
        {
            return db.size();
        }

        const species::knot *library:: head() const throw()
        {
            return (*db.tree).head;
        }

        const species & library:: operator[](const size_t j) const throw()
        {
            assert(j>=1);
            assert(j<=size());
            const list_of<snode> &l = *db.tree;
            const snode          *s = l.get(j);
            return ***s;
        }

        static const char fn[] = "chemical::library";

        const species &library:: check(const species::pointer &ptr)
        {
            //------------------------------------------------------------------
            // try to lock access
            //------------------------------------------------------------------
            if(!try_lock()) throw exception("%s is locked, cannot declare '%s'", fn, ptr->name());
            const return_unlocked   ___(*this);

            //------------------------------------------------------------------
            // look for same name
            //------------------------------------------------------------------
            const string           &key = ptr->name;
            const species::pointer *pps = db.search(key);
            if(pps)
            {
                //--------------------------------------------------------------
                // already registered
                //--------------------------------------------------------------
                const species::pointer &the = *pps;
                if( the->z != ptr->z ) throw exception("%s charge mismatch for '%s'",fn,key());
                return *the; //!< already declared species
            }
            else
            {
                //--------------------------------------------------------------
                // new species
                //--------------------------------------------------------------
                if(!db.insert(ptr)) throw exception("%s unable to insert '%s'",fn,key());
                coerce(width)      = max_of(width,key.size());
                update(coerce(*ptr),db.size());
                return *ptr; //!< the passed-on species
            }

        }

        size_t library:: active() const throw()
        {
            size_t ans = 0;
            for(const snode *node=head();node;node=node->next)
            {
                const species &sp = ***node;
                if(sp.rank>0) ++ans;
            }
            return ans;
        }
        
        const species & library:: operator()(const string &expr)
        {
            static builder         &mgr = builder::instance();
            const species::pointer  ptr = mgr.compile(expr);
            return check(ptr);
        }


        

        void library:: load(const string &splist)
        {
            vector<string> words;
            tokenizer::split_with(separator,words,splist);
            for(size_t i=1;i<=words.size();++i)
            {
                (void) (*this)(words[i]);
            }
        }

        void library:: load(const char *splist)
        {
            const string _(splist);
            load(_);
        }

        const species & library:: operator()(const char *expr)
        {
            const string _(expr);
            return (*this)(_);
        }

        std::ostream & operator<<(std::ostream &os, const library &lib)
        {
            lib.display(os);
            return os;
        }

        const species & library:: operator[](const string &name) const
        {
            const species::pointer *pps = db.search(name);
            if(!pps) throw exception("no library['%s']", name());
            return **pps;
        }

        const species & library:: operator[](const char *name) const
        {
            const string _(name); return (*this)[_];
        }

        void library:: fill(writable<double> &C,
                            const double      zabove,
                            randomized::bits &ran) throw()
        {
            for(const snode *node=head();node;node=node->next)
            {
                const species &sp = ***node;
                double         sg = 1;
                if(sp.rank<=0)
                {
                    if(ran.choice()) sg = -1;
                }

                if(ran.to<double>()>zabove)
                {
                    sp(C) = 0;
                }
                else
                {
                    sp(C) = sg * species::concentration(ran);
                }

            }
        }


    }

}

#include <iomanip>

namespace yack
{

    namespace chemical
    {



        void library:: display(std::ostream &os, const species &sp) const
        {
            os << '[' << sp.name << ']';
            pad(os,sp.name.size());
        }

        void library:: display(std::ostream &os) const
        {
            os << '{' << std::endl;
            for(const species::knot *node=head();node;node=node->next)
            {
                const species &sp = ***node;
                display(os << "  ",sp);
                os << ':';
                os << " @"     << std::setw(3) << *sp;
                os << " z="    << std::setw(3) << sp.z;
                os << " rank=" << std::setw(3) << sp.rank;
                os << std::endl;
            }
            os << '}';
        }

    }
}

