
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

            //------------------------------------------------------------------
            //
            // process top-level
            //
            //------------------------------------------------------------------
            assert(top); assert("MODULE" == *(**top).name);

            syntax::xlist      &self = top->sub();
            syntax::xlist       temp;
            size_t              nr = 0;
            while(self.size)
            {
                syntax::xnode      *node = self.head; assert(node);
                const syntax::rule &from = **node;
                const string       &name = *from.name;
                switch( top2type(name) )
                {
                    case _inc: assert("INC"==name);
                        temp.push_back( compile(open_from(node->head()->lex())) );
                        delete self.pop_front();
                        continue;

                    case _rule: ++nr; assert("RULE"==name);
                    default:
                        break;
                }
                temp.push_back( self.pop_front() );
            }
            self.swap_with(temp);
            std::cerr << "\t\t#rule = " << nr << std::endl;
        }

        syntax::xnode *dsl_parser:: compile(module *m)
        {
            auto_ptr<syntax::xnode> tree = parse(m);
            assert(tree.is_valid());
            preprocess(& *tree);
            return tree.yield();
        }


    }

}


