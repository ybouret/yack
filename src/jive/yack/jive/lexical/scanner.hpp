//! \file

#ifndef YACK_LEXICAL_SCANNER_INCLUDED
#define YACK_LEXICAL_SCANNER_INCLUDED 1

#include "yack/jive/lexical/directive.hpp"
#include "yack/jive/lexical/lexeme.hpp"
#include "yack/jive/pattern/scatter-table.hpp"
#include "yack/jive/pattern/regexp.hpp"

namespace yack
{
    namespace jive
    {

        namespace lexical
        {

            //__________________________________________________________________
            //
            //
            //! lexical scanner to produce lexemes
            //
            //__________________________________________________________________
            class scanner : public object
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~scanner() throw(); //!< cleanup

                //! setup with optional dictionary
                template <typename LABEL> inline
                explicit scanner(const LABEL &id, const dictionary *user_dict) :
                object(),
                label( tags::make(id)       ),
                instr( new instructions()   ),
                table( new scatter::table() ),
                dict_(user_dict)
                {}

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! register a new directive
                void make(const tag &t, const motif &m, const action &a);

                //! generic helper to build directive
                template <
                typename IDENTIFIER,
                typename EXPRESSION,
                typename OBJECT_POINTER,
                typename METHOD_POINTER
                >
                inline void make(const IDENTIFIER     &uuid,
                                 const EXPRESSION     &expr,
                                 const OBJECT_POINTER  host,
                                 const METHOD_POINTER  meth)
                {
                    assert(host);
                    assert(meth);
                    const tag    t = tags::make(uuid);
                    const action a(host,meth);
                    const motif  m(regexp::compile(expr,dict_) );
                    make(t,m,a);
                }

                //! default emit method
                bool    emit(const token &) throw();

                //! probe next lexeme
                lexeme *probe(source &source);

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const tag                       label; //!< scanner label
            private:
                auto_ptr<instructions>          instr;
                auto_ptr<scatter::table>        table;
                const dictionary               *dict_;
                YACK_DISABLE_COPY_AND_ASSIGN(scanner);
            };
        }

    }
}

#endif

