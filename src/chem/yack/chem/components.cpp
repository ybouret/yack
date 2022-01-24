#include "yack/chem/components.hpp"


namespace yack
{

    namespace chemical
    {

        components:: ~components() throw()
        {
        }

        components:: components() throw() : components_()
        {
        }
    }

}

#include "yack/chem/builder.hpp"

namespace yack
{

    namespace chemical
    {

        void components:: operator()(const string &expr, library &lib)
        {
            static builder &mgr = builder::instance();
            mgr.compile(*this,expr,lib);
        }

    }

}

