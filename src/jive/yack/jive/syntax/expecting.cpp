
#include "yack/jive/syntax/expecting.hpp"
#include "yack/jive/syntax/rule/all.hpp"
#include "yack/associative/addrbook.hpp"

namespace yack
{

    namespace jive
    {

        namespace syntax
        {

            expected:: expected(const terminal &t) throw() :
            object(), authority<const terminal>(t), next(0), prev(0)
            {
            }

            expected:: ~expected() throw()
            {
            }


            struct expecting
            {
                static
                void to( expected::list &xp, const rule &r, addrbook &db )
                {
                    const uint32_t uuid = r.uuid;
                    switch(uuid)
                    {
                        case terminal::mark:
                            db.ensure(&r);
                            break;

                        case option::mark:
                            if(db.insert(&r))
                                to(xp, **(r.as<option>()), db);
                            break;

                        case repeat::mark:
                            if(db.insert(&r))
                                to(xp, **(r.as<repeat>()), db);
                            break;

                        case alternate::mark:
                            if(db.insert(&r))
                            {
                                
                            }
                            break;

                        case aggregate::mark:

                            break;
                    }
                }
            };

        }

    }

}

