
#include "yack/json/parser.hpp"
#include "yack/jive/lexical/plugin/jstring.hpp"

namespace yack
{

    namespace JSON
    {

        Parser:: ~Parser() throw()
        {
        }

        Parser:: Parser() : jive::parser("JSON")
        {
            compound &self = alt("JSON");

            compound   &VALUE  = alt("value");
            const rule &STRING = load<jive::lexical::jstring>("string");
            const rule &NUMBER = term("number","[-+]?[:digit:]+(\\.[:digit:]*)?");
            VALUE << STRING << NUMBER << term("null") << term("true") << term("false");
            const rule &COMMA = mark(',');

            {
                compound   &ARRAY = alt("array");
                {
                    const rule &LBRACK = mark('[');
                    const rule &RBRACK = mark(']');
                    ARRAY << ( agg("empty_array") << LBRACK << RBRACK );
                    ARRAY << ( agg("heavy_array") << LBRACK << VALUE << zom( cat(COMMA,VALUE) ) <<  RBRACK );
                }
                self  << ARRAY;
                VALUE << ARRAY;
            }

            {
                compound   &OBJECT = alt("object");
                {
                    const rule &LBRACE = mark('{');
                    const rule &RBRACE = mark('}');
                    OBJECT << ( agg("empty_object") << LBRACE << RBRACE );
                    {
                        const rule &PAIR = ( agg("PAIR") << STRING << mark(':') << VALUE );
                        OBJECT << ( agg("heavy_object") << LBRACE << PAIR << zom( cat(COMMA,PAIR) ) << RBRACE);
                    }
                }
                self  << OBJECT;
                VALUE << OBJECT;
            }

            drop("[:blank:]");
            endl("[:endl:]");

            gv();
            validate();

        }

    }

}

