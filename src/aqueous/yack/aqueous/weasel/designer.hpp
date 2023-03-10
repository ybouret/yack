
//! \file

#ifndef YACK_AQUEOUSE_WEASEL_DESIGNER_INCLUDED
#define YACK_AQUEOUSE_WEASEL_DESIGNER_INCLUDED 1

#include "yack/aqueous/library.hpp"
#include "yack/aqueous/lua/equilibria.hpp"
#include "yack/jive/module.hpp"
#include "yack/singleton.hpp"

namespace yack
{
    namespace aqueous
    {

        namespace weasel
        {

            class designer : public singleton<designer>
            {
            public:

                static const at_exit::longevity life_time = 1027;
                static const char               call_sign[];       //!< weasel::designer
                class compiler;
                static const char * const       edb[];
                static const size_t             ndb;

                void operator()(library        &lib,
                                lua_equilibria &eqs,
                                jive::module   *inp);


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(designer);
                explicit designer();
                virtual ~designer() noexcept;
                friend class singleton<designer>;
                compiler *impl;
            };

        }

    }

}

#endif
