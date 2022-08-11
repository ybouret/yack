
#include "yack/chem/designer.hpp"
#include "yack/hashing/perfect.hpp"
#include "yack/system/imported.hpp"

#include "yack/chem/designer/sp-parser.hpp"
#include "yack/chem/designer/sp-linker.hpp"

#include "yack/chem/designer/cm-parser.hpp"


namespace yack
{
    using namespace jive;

    namespace chemical
    {


        const char * const designer:: call_sign = "chemical::designer";

        designer:: ~designer() throw()
        {
        }



        designer:: designer() :
        spp( new nucleus::sp_parser() ),
        spl( new nucleus::sp_linker() ),

        cmp( new nucleus::cm_parser() )
        {
        }

    }

}

//------------------------------------------------------------------------------
//
// SPECIES
//
//------------------------------------------------------------------------------
namespace yack
{

    namespace chemical
    {
        nucleus::sp_info designer:: species_info_from(jive::module *m)
        {
            source src(m);
            spp->reset();
            auto_ptr<syntax::xnode> code  = spp->parse(src);
            nucleus::sp_info        info;
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

        const species & designer:: species_from(library &lib, const nucleus::sp_info &info) const
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
        

        const species & library:: operator()(const string &expr)
        {
            static designer &io = designer::instance();
            return io(*this,expr);
        }

        const species & library:: operator()(const char *expr)
        {
            static designer &io = designer::instance();
            return io(*this,expr);
        }



    }

}
