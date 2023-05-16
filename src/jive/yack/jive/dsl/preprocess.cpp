
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
        
        
        void dsl_parser:: process_(syntax::xnode *top, size_t &nr)
        {

            //------------------------------------------------------------------
            //
            // process top level list a.k.a "MODULE"
            //
            //------------------------------------------------------------------
            assert(top); assert("MODULE" == top->name() );
            assert(0==nr);
            {
                syntax::xlist      &self = top->sub();
                syntax::xlist       temp;
                while(self.size)
                {
                    syntax::xnode      *node = self.head; assert(node);
                    const string       &name = node->name();
                    if( "INC" == name )
                    {
                        size_t                  xr = 0;
                        auto_ptr<syntax::xnode> xn = compile_(open_from(node->head()->lex()),xr);
                        if(xr<=0)
                        {
                            syntax::xlist &sub = xn->sub();
                            delete sub.pop_front();
                            temp.merge_back( sub );
                            //temp.push_back( xn.yield() );
                        }
                        else
                        {
                            temp.push_back( xn.yield() );
                        }
                        delete self.pop_front();
                    }
                    else
                    {
                        if("RULE" == name) ++nr;
                        temp.push_back( self.pop_front() );
                    }
                }
                self.swap_with(temp);
            }

        }

        syntax::xnode *dsl_parser:: compile_(module *m, size_t &nr)
        {
            assert(0==nr);
            auto_ptr<syntax::xnode> tree = parse(m);
            assert(tree.is_valid());
            process_(& *tree, nr);
            return tree.yield();
        }

        syntax::xnode *dsl_parser:: compile(module *m)
        {
            size_t                  nr = 0;
            auto_ptr<syntax::xnode> tree = compile_(m,nr);
            std::cerr << "\t-------- top-module #rule=" <<  nr << std::endl;
            if(nr<=0)
                throw imported::exception( (*label)(), "no rules in top-level module");
            return tree.yield();
        }


    }

}


