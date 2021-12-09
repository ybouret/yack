#include "yack/jive/pattern/all.hpp"
#include "yack/exception.hpp"
#include "yack/type/fourcc.h"


namespace yack
{
    namespace jive
    {


        bool pattern:: is_empty(const pattern *p)
        {
            switch(p->uuid)
            {
                case single::mark:
                case within::mark:
                case except::mark:
                    return false;

                case optional::mark: return is_empty( & **(p->as<optional>()) );
                case at_least::mark: return is_empty( & **(p->as<at_least>()) );
                case counting::mark: return is_empty( & **(p->as<counting>()) );

                case op_and:: mark:   return p->as<op_and>()->size  <= 0;
                case op_or::  mark:   return p->as<op_or>()->size   <= 0;
                case op_none::mark:   return p->as<op_none>()->size <= 0;

                default:
                    throw exception("pattern::is_empty(unhandled [%s])", yack_fourcc(p->uuid));
            }
        }

        bool pattern:: is_empty() const
        {
            return is_empty(this);
        }

    }

}

