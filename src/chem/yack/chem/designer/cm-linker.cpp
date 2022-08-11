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


            static const char * cm_term[] =
            {
                COM::SP_NAME, //!< 0
                COM::COEF,    //!< 1
                "+",          //!< 2
                "-"           //!< 3
            };




            cm_linker:: cm_linker() : syntax::translator(),
            thash( YACK_HASHING_PERFECT(cm_term) ),
            ilist(8,as_capacity)
            {
            }

            cm_linker:: ~cm_linker() throw()
            {
            }

            void cm_linker:: on_init()
            {
                //std::cerr << "cm_linker.on_init" << std::endl;
                ilist.free();
            }

            

            void cm_linker:: on_terminal(const lexeme &lx)
            {
                translator::on_terminal(lx);
                const string &uid = *(lx.name);

                switch( thash(uid) )
                {
                    case 0: assert(uid==cm_term[0]);
                        break;

                    case 1: assert(uid==cm_term[1]);
                    {
                        const apn ap_coef = lx.data.to_apn(cm_term[1]);
                        ilist << ap_coef.cast_to<int>(cm_term[1]);
                    }
                        std::cerr << "ilist=" << ilist << std::endl;
                        break;

                    case 2: assert(uid==cm_term[2]);
                        ilist << 1;
                        std::cerr << "ilist=" << ilist << std::endl;
                        break;

                    case 3: assert(uid==cm_term[3]);
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

        }
    }
}

