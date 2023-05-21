
//! \file

#ifndef YACK_JIVE_SYNTAX_TRANSCRIBER_INCLUDED
#define YACK_JIVE_SYNTAX_TRANSCRIBER_INCLUDED 1

#include "yack/jive/syntax/translator.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {


#define YACK_JIVE_TRANSFN(FN,UUID,CLASS) FN(#UUID, static_cast<CLASS*>(this), & CLASS:: on_##UUID) //!< helper
#define YACK_JIVE_CONNECT(UUID,CLASS) YACK_JIVE_TRANSFN(connect,UUID,CLASS) //!< helper
#define YACK_JIVE_CONTROL(UUID,CLASS) YACK_JIVE_TRANSFN(control,UUID,CLASS) //!< helper

            //__________________________________________________________________
            //
            //
            //! transcriber to process AST by event
            /**
             designed to be derived to compile AST, C++ style
             */
            //
            //__________________________________________________________________
            class transcriber : public translator
            {
            public:
                //______________________________________________________________
                //
                // definitions
                //______________________________________________________________
                typedef functor<void,TL1(const lexeme &)>         terminal_event; //!< event for terminal
                typedef suffix_map<string,terminal_event>         terminal_table; //!< table of events
                typedef functor<void,TL2(const string &,size_t)>  internal_event; //!< event for internal
                typedef suffix_map<string,internal_event>         internal_table; //!< table of events

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup with label
                template <typename LABEL>
                explicit transcriber(const LABEL &id, bool verb=false) :
                label( tags::make(id) ), tmap(), imap(), verbose(verb)
                {
                }
                virtual ~transcriber() noexcept;

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //______________________________________________________________
                //
                //! connect terminals to methods
                //______________________________________________________________
                template <typename IDENTIFIER, typename OBJECT_POINTER, typename METHOD_POINTER>
                void connect(const IDENTIFIER  &uuid,
                             OBJECT_POINTER    *host,
                             METHOD_POINTER     meth)
                {
                    const terminal_event ev(host,meth);
                    connect_(uuid,ev);
                }


                //______________________________________________________________
                //
                //! control internals with methods
                //______________________________________________________________
                template <typename IDENTIFIER, typename OBJECT_POINTER, typename METHOD_POINTER>
                inline void control(const IDENTIFIER  &uuid,
                                    OBJECT_POINTER    *host,
                                    METHOD_POINTER     meth)
                {
                    const internal_event ev(host,meth);
                    control_(uuid,ev);
                }

                



                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const tag label; //!< label I/O

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(transcriber);
                virtual void on_terminal(const lexeme &);
                virtual void on_internal(const string &, const size_t);

                terminal_table tmap;
                internal_table imap;

                void connect_(const string &uuid, const terminal_event &ev);
                void connect_(const char   *uuid, const terminal_event &ev);

                void control_(const string &uuid, const internal_event &ev);
                void control_(const char   *uuid, const internal_event &ev);

            public:
                bool verbose; //!< display process flag
            };

        }

    }

}

#endif

