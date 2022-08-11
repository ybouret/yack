#include "yack/chem/designer/cm-linker.hpp"
#include "yack/chem/designer/sp-info.hpp"
#include "yack/chem/designer/com.hpp"
#include "yack/exception.hpp"
#include "yack/apex/natural.hpp"

namespace yack
{
    using namespace jive;

    namespace chemical
    {
        namespace nucleus
        {


            static const char * cm_terminals[] =
            {
                COM::SP_NAME, //!< 0
                COM::COEF,    //!< 1
                "+",          //!< 2
                "-"           //!< 3
            };

            static const char * cm_internals[] =
            {
                COM::COMPONENTS, //!< 0
                COM::SPECIES,    //!< 1
                COM::FIRST_COEF, //!< 2
                COM::EXTRA_COEF  //!< 3
            };




            cm_linker:: cm_linker() : syntax::translator(),
            ilist(8,as_capacity),
            sname(),
            thash( YACK_HASHING_PERFECT(cm_terminals) ),
            ihash( YACK_HASHING_PERFECT(cm_internals) )
            {
            }

            cm_linker:: ~cm_linker() throw()
            {
            }

            void cm_linker:: on_init()
            {
                ilist.free();
            }

            

            void cm_linker:: on_terminal(const lexeme &lx)
            {
                translator::on_terminal(lx);
                const string &uid = *(lx.name);

                switch( thash(uid) )
                {
                    case 0: assert(uid==COM::SP_NAME);
                        sname = lx.data.to_string();
                        break;

                    case 1: assert(uid==COM::COEF);
                    {
                        const apn ap_coef = lx.data.to_apn(cm_terminals[1]);
                        ilist << ap_coef.cast_to<int>(cm_terminals[1]);
                    }
                        std::cerr << "ilist=" << ilist << std::endl;
                        break;

                    case 2: assert(uid=="+");
                        ilist << 1;
                        std::cerr << "ilist=" << ilist << std::endl;
                        break;

                    case 3: assert(uid=="-");
                        ilist << -1;
                        std::cerr << "ilist=" << ilist << std::endl;
                        break;

                    default:
                        raise_error(uid);
                }

            }

            void cm_linker:: raise_error(const string &word)
            {
                throw  exception("corrupted components linker with <'%s'>",word());
            }

            
            void cm_linker:: reduce() throw()
            {

                assert(ilist.size()>=2);
                const int tmp = ilist.pull_back();
                ilist.back() *= tmp;
                std::cerr << "ilist=" << ilist << std::endl;
            }

            void  cm_linker :: on_internal(const string &uid,const size_t n)
            {
                translator::on_internal(uid,n);

                switch( ihash(uid) )
                {
                    case 0: assert(uid==COM::COMPONENTS);
                        break;

                    case 1: assert(uid==COM::SPECIES);
                    {
                        assert(ilist.size()>=n-1);
                        int z = 0;
                        for(size_t i=n;i>1;--i)
                        {
                            z += ilist.pull_back();
                        }
                        int nu = 1;
                        if(ilist.size()) nu = ilist.pull_back();
                        std::cerr << "name = " << sname << std::endl;
                        std::cerr << "z    = " << z << std::endl;
                        std::cerr << "nu   = " << nu << std::endl;

                    } break;

                    case 2: assert(uid==COM::FIRST_COEF);
                        switch(n)
                        {
                            case 1:
                                break;

                            case 2:
                                reduce();
                                break;

                            default:
                                raise_error(uid);
                        }
                        break;




                    case 3: assert(uid==COM::EXTRA_COEF);
                        switch(n)
                        {
                            case 1:
                                break;

                            case 2:
                                reduce();
                                break;

                            default:
                                raise_error(uid);
                        }
                        break;

                    default:
                        raise_error(uid);
                }

            }

        }
    }
}

