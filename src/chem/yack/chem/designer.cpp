
#include "yack/chem/designer.hpp"
#include "yack/hashing/perfect.hpp"
#include "yack/system/imported.hpp"

#include "yack/chem/designer/sp-parser.hpp"
#include "yack/chem/designer/sp-linker.hpp"

#include "yack/chem/designer/cm-parser.hpp"
#include "yack/chem/designer/cm-linker.hpp"


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

        cmp( new nucleus::cm_parser() ),
        cml( new nucleus::cm_linker() )
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
        nucleus::sp_info designer:: sp_info_from(jive::module *m)
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

//------------------------------------------------------------------------------
//
// COMPONENTS
//
//------------------------------------------------------------------------------
namespace yack
{

    namespace chemical
    {
        void designer:: cm_infos_from(jive::module *m, nucleus::cm_infos &target)
        {
            source src(m);
            cmp->reset();
            auto_ptr<syntax::xnode> code  = cmp->parse(src);
            cml->walk(*code,&target);
        }

        void designer:: components_to(components              &cmp,
                                      library                 &lib,
                                      const nucleus::cm_infos &cmi)
        {
            for(nucleus::cm_infos::const_iterator it=cmi.begin();it!=cmi.end();++it)
            {
                const nucleus::cm_info &cm = *it;
                const species          &sp = species_from(lib,cm);
                cmp(sp,cm.nu);
            }
        }

        void components:: operator()(library &lib, const string &data)
        {
            static designer &io = designer::instance();
            io(*this,lib,data);
        }

        void components:: operator()(library &lib, const char *data)
        {
            static designer &io = designer::instance();
            io(*this,lib,data);
        }



    }

}
