
//! \file

#ifndef YACK_JIVE_LEXICAL_VERBATIM_INCLUDED
#define YACK_JIVE_LEXICAL_VERBATIM_INCLUDED 1

#include "yack/jive/lexical/plugin.hpp"

namespace yack
{
    namespace jive
    {

        namespace lexical
        {

            //__________________________________________________________________
            //
            //
            //! create a verbatim block extractor
            //
            //__________________________________________________________________
            class verbatim : public plugin
            {
            public:
                YACK_JIVE_PLUGIN_DECL(verbatim); //!< alias

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~verbatim() noexcept; //!< cleanup

                //! setup with enter/leave char
                template <
                typename IDENTIFIER,
                typename EXPRESSINI,
                typename EXPRESSEND
                > inline
                explicit verbatim(const IDENTIFIER &uuid,
                                  const EXPRESSINI &init,
                                  const EXPRESSEND &done,
                                  analyzer         &host) :
                plugin(uuid,init,host,reject_eos),
                content(),
                to_skip(0)
                {
                    back(done,this,&verbatim::leave);
                    setup();
                }

                //______________________________________________________________
                //
                // membes
                //______________________________________________________________
                token        content; //!< accumulator
                const size_t to_skip; //!< chars to skip

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(verbatim);
                void         setup();
                virtual void enter(token &) noexcept;
                void         leave(token &);
                behavior     on_dot(token &) noexcept;
                behavior     on_endl(token &) noexcept;
            };

        }
    }
}

#endif
