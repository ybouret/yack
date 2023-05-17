#include "yack/jive/syntax/transcriber.hpp"
#include "yack/system/imported.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            transcriber:: ~transcriber() noexcept {}

            

            
            void transcriber:: on_terminal(const lexeme &lxm)
            {
                const string         &key = *lxm.name;
                const terminal_event *evt = tmap.search(key);
                if(!evt)
                    throw imported::exception( (*label)(),"no connection to [%s]", key() );
                coerce(*evt)(lxm);

            }

            void transcriber:: on_internal(const string &uuid, const size_t narg)
            {
                const internal_event *evt = imap.search(uuid);
                if(!evt)
                    throw imported::exception( (*label)(),"no control for [%s]", uuid() );
                coerce(*evt)(uuid,narg);

            }



            void transcriber:: connect_(const string &uuid, const terminal_event &ev)
            {
                if(!tmap.insert(uuid,ev))
                    throw imported::exception( (*label)(),"multiple connect to [%s]", uuid() );
            }


            void transcriber:: connect_(const char *uuid, const terminal_event &ev)
            {
                const string _(uuid); connect_(_,ev);
            }

            void transcriber:: control_(const string &uuid, const internal_event &ev)
            {
                if(!imap.insert(uuid,ev))
                    throw imported::exception((*label)(),"multiple contol by [%s]", uuid() );
            }

            void transcriber:: control_(const char *uuid, const internal_event &ev)
            {
                const string _(uuid); control_(_,ev);
            }
        }

    }

}

