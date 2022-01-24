
#include "yack/chem/library.hpp"
#include "yack/exception.hpp"
#include "yack/chem/builder.hpp"

namespace yack
{

    namespace chemical
    {

        library:: ~library() throw() {}

        library:: library() : db()
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

        void library:: decl(const species::pointer &sp)
        {
            if(try_lock())
            {
                const return_unlocked _(*this);
                if(!db.insert(sp)) throw exception("multiple species '%s'", sp->name() );
            }
            else
            {
                throw exception("library is locked, cannot declare '%s'", sp->name());
            }
        }

        const species & library:: operator()(const string &expr)
        {
            static builder        &mgr = builder::instance();
            const species::pointer ptr = NULL;
            decl(ptr);
            return *ptr;
        }

        const species & library:: operator()(const char *expr)
        {
            const string _(expr);
            return (*this)(_);
        }


    }
}

