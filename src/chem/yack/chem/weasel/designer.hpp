
//! \file

#ifndef YACK_CHEMICAL_WEASEL_DESIGNER_INCLUDED
#define YACK_CHEMICAL_WEASEL_DESIGNER_INCLUDED 1

#include "yack/chem/library.hpp"
#include "yack/chem/lua/equilibria.hpp"
#include "yack/jive/module.hpp"
#include "yack/singleton.hpp"

namespace yack
{
    namespace chemical
    {

        namespace weasel
        {
            //__________________________________________________________________
            //
            //
            //! database and compiler
            //
            //__________________________________________________________________
            class designer : public singleton<designer>
            {
            public:
                //______________________________________________________________
                //
                // definitions
                //______________________________________________________________
                static const at_exit::longevity life_time = 1027; //!< for singletong
                static const char               call_sign[];      //!< weasel::designer
                class compiler;
                static const char * const       edb[]; //!< internal equilibria description
                static const size_t             ndb;   //!< |edb|

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! populate lib and eqs from input
                void operator()(library        &lib,
                                lua_equilibria &eqs,
                                jive::module   *inp);

                //! check each database entry
                void checkDB();

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
