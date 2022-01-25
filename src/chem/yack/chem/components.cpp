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


        bool components:: add(const species  &sp,
                              const unit_t    nu)
        {
            const component::pointer p = new component(sp,nu);
            return insert(p);
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

