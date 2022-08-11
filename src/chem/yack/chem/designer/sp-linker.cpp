#include "yack/chem/designer/sp-linker.hpp"
#include "yack/chem/designer/sp-info.hpp"
#include "yack/exception.hpp"

namespace yack
{
    using namespace jive;
    
    namespace chemical
    {
        namespace nucleus
        {


            static const char * sp_term[] =
            {
                "NAME", //!< 0
                "+",    //!< 1
                "-"     //!< 2
            };

            sp_linker:: sp_linker() : syntax::translator(),
            thash( YACK_HASHING_PERFECT(sp_term) )
            {
            }

            sp_linker:: ~sp_linker() throw()
            {
            }
            

            void sp_linker:: raise_error(const string &word)
            {
                throw  exception("corrupted species linker with <%s>",word());
            }

            void sp_linker:: on_terminal(const lexeme &lxm)
            {
                assert(NULL!=data);
                const string &tid = *lxm.name;
                sp_info      &msg = *static_cast<sp_info *>(data);

                switch( thash(tid) )
                {
                    case 0: assert(sp_term[0]==tid);
                        msg.name = lxm.data.to_string();
                        break;

                    case 1:assert(sp_term[1]==tid);
                        ++msg.z;
                        break;

                    case 2:assert(sp_term[2]==tid);
                        --msg.z;
                        break;

                    default:
                        raise_error(tid);
                }
            }

            void sp_linker:: on_internal(const string &uid, const size_t n)
            {
                assert(NULL!=data);
                if(uid!="SPECIES") raise_error(uid);
                sp_info &msg = *static_cast<sp_info *>(data);
                if(n != size_t(abs(msg.z)+1) ) raise_error(msg.name);
            }

        }
    }

}
