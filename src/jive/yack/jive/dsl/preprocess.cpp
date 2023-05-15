
#include "yack/jive/dsl/parser.hpp"
#include "yack/system/imported.hpp"
#include "yack/fs/vfs.hpp"

namespace yack
{
    namespace jive
    {
        module * dsl_parser:: open_from(const lexeme &lxm)
        {
            const string        &lid = *lxm.name;   assert("BSTRING"==lid||"JSTRING"==lid);
            const string        &fn  = lxm.data.to_string(1,1);
            std::cerr << " file=[" << fn << "]" << std::endl;
            switch( str2type(lid) )
            {
                case _jstring: {
                    const string &gfile = *lxm;
                    std::cerr << "gfile=[" << gfile << "]" << std::endl;
                    if( gfile== YACK_STDIN )
                    {
                        return jive::module::open_file( fn );
                    }
                    else
                    {
                        const string gdir = vfs::get_dir_name(gfile);
                        const string ginc = gdir + fn;
                        return jive::module::open_file( ginc );
                    }

                } return 0;

                case _bstring:
                    throw imported::exception( (*label)(), "not implemented %s %s=[%s]", _include, lid(), fn());

                default:
                    throw imported::exception( (*label)(), "unexpected %s %s=[%s]", _include, lid(), fn());
            }
        }

        void dsl_parser:: preprocess(syntax::xnode *top)
        {

            assert(top);
            assert("MODULE" == *(**top).name);
            syntax::xlist      &self = top->sub();
            syntax::xlist       temp;
            while(self.size)
            {
                syntax::xnode      *node = self.head; assert(node);
                const syntax::rule &from = **node;
                if( *from.name == "INC" )
                {
                    std::cerr << "found INC" << std::endl;
                    temp.push_back( parse(open_from(node->head()->lex())) );
                    delete self.pop_front();

                }
                else
                {
                    temp.push_back( self.pop_front() );
                }
            }
            self.swap_with(temp);
        }

    }

}


