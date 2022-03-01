

#include "yack/chem/library.hpp"

namespace yack
{
    namespace chemical
    {

        library:: ~library() throw()
        {
        }

        library:: library() throw()
        {
        }


        size_t library:: size() const throw()
        {
            return db.size();
        }

        const snode * library:: head() const throw()
        {
            return (*db.tree).head;
        }

    }

}
