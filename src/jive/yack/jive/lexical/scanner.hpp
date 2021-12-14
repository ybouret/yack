//! \file

#ifndef YACK_LEXICAL_SCANNER_INCLUDED
#define YACK_LEXICAL_SCANNER_INCLUDED 1

#include "yack/jive/pattern/scatter-table.hpp"
#include "yack/jive/pattern/regexp.hpp"
#include "yack/functor.hpp"

namespace yack
{
    namespace jive
    {
        class dictionary;

        namespace lexical
        {

            typedef functor<bool,TL1(const token &)> action;

            class directive : public object
            {
            public:
                virtual ~directive() throw();
                explicit directive(const motif &, const action &);

                directive   *next;
                const motif  info;
                const action todo;


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(directive);
            };

            typedef cxx_pool_of<directive> directives;


            class scanner : public object
            {
            public:
                virtual ~scanner() throw();
                template <typename LABEL> inline
                explicit scanner(const LABEL &id, const dictionary *user_dict) :
                object(),
                label( tags::make(id) ),
                instr(),
                table( new scatter::table() ),
                dict_(user_dict)
                {}

                void make(pattern *p, const action &a);

                template <
                typename EXPRESSION,
                typename OBJECT_POINTER,
                typename METHOD_POINTER>
                inline void operator()(const EXPRESSION     &expr,
                                       const OBJECT_POINTER host,
                                       const METHOD_POINTER meth)
                {
                    assert(host);
                    assert(meth);
                    const action a(host,meth);
                    make( regexp::compile(expr,dict_), a);
                }

                const tag                      label;
            private:
                directives                     instr;
                auto_ptr<scatter::table>       table;
                const dictionary              *dict_;
                YACK_DISABLE_COPY_AND_ASSIGN(scanner);
            };
        }

    }
}

#endif

