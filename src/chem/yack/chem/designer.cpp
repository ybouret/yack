
#include "yack/chem/designer.hpp"
#include "yack/hashing/perfect.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    using namespace jive;

    namespace chemical
    {

        species_info::  species_info() : name(), z(0) {}
        species_info::  species_info(const species_info &si) : name(si.name), z(si.z) {}
        species_info:: ~species_info() throw() {}


        const char * const designer:: call_sign = "chemical::designer";

        designer:: ~designer() throw()
        {
        }


        namespace
        {

            static const char * sp_term[] =
            {
                "NAME", //!< 0
                "+",    //!< 1
                "-"     //!< 2
            };


            class sp_linker  : public large_object, public syntax::translator
            {
            public:
                const hashing::perfect thash;

                explicit sp_linker() : syntax::translator(),
                thash( YACK_HASHING_PERFECT(sp_term) )
                {
                }

                virtual ~sp_linker() throw() {}



                virtual void on_terminal(const lexeme &lxm)
                {
                    assert(NULL!=data);
                    const string &tid = *lxm.name;
                    species_info &msg = *static_cast<species_info *>(data);

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

                virtual void on_internal(const string &uid, const size_t n)
                {
                    assert(NULL!=data);
                    if(uid!="SPECIES") raise_error(uid);
                    species_info &msg = *static_cast<species_info *>(data);
                    if(n != size_t(abs(msg.z)+1) ) raise_error(msg.name);

                }


                static inline void raise_error(const string &word)
                {
                    throw imported::exception(designer::call_sign,"corrupted species linker with <%s>",word());
                }



            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sp_linker);
            };

            class sp_parser : public   parser
            {
            public:
                inline virtual ~sp_parser() throw() {}

                inline explicit sp_parser() : jive:: parser("chemical::species::parser")
                {
                    syntax::compound &SPECIES = agg("SPECIES");
                    SPECIES << term("NAME", "[:upper:][:word:]*");
                    {
                        const syntax::rule &POS = oom( term('+') );
                        const syntax::rule &NEG = oom( term('-') );
                        SPECIES << opt( choice(POS,NEG) );
                    }
                    //gv();
                }
                

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sp_parser);
            };
        }

        designer:: designer() :
        spp( new sp_parser() ),
        spl( new sp_linker() )
        {
        }


        species_info designer:: species_info_from(jive::module *m)
        {
            source src(m);
            spp->reset();
            auto_ptr<syntax::xnode> code  = spp->parse(src);
            species_info            info;
            spl->walk(*code,&info);
            return info;
        }


        static inline void add_sign(string &uuid, const int z)
        {
            if(z<0)
            {
                for(int i = -z;  i>0; --i) uuid += '-';
            }
            else
            {
                if(z>0)
                {
                    for(int i=z;i>0;--i) uuid += '+';
                }
            }
        }

        const species & designer:: species_from(library &lib, const species_info &info) const
        {
            const string   uuid = info.name; add_sign( coerce(uuid),info.z);
            const species *sptr = lib.look_up(uuid);
            if(sptr)
            {
                if(sptr->z!=info.z) throw imported::exception(call_sign,"charge mismatch for '%s'", uuid());
                return *sptr;
            }
            else
            {
                return lib(uuid,info.z);
            }
        }


    }

}
