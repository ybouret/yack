
//! \file

#ifndef YACK_CHEMICAL_DESIGNER_INCLUDED
#define YACK_CHEMICAL_DESIGNER_INCLUDED 1

#include "yack/chem/library.hpp"

#include "yack/singleton.hpp"

#include "yack/jive/parser.hpp"
#include "yack/jive/syntax/translator.hpp"

namespace yack
{
    namespace chemical
    {

        class species_info
        {
        public:
            species_info();
            ~species_info() throw();
            species_info(const species_info &);

            string name;
            int    z;
            

        private:
            YACK_DISABLE_ASSIGN(species_info);
        };

        class designer : public singleton<designer>
        {
        public:
            typedef jive::parser              parser;
            typedef jive::syntax::translator  linker;

            static const char * const       call_sign; //!< "chemical::designer"
            static const at_exit::longevity life_time = 90;


            template <typename DATA> inline
            const species & operator()(library &lib, const DATA &data)
            {
                const  species_info info = get_species_info(data);
                return species_from(lib,info);
            }




        private:
            YACK_DISABLE_COPY_AND_ASSIGN(designer);
            friend class singleton<designer>;

            explicit designer();
            virtual ~designer() throw();

            auto_ptr<parser> spp;
            auto_ptr<linker> spl;
            
            template <typename DATA> inline
            species_info get_species_info(const DATA &data) {
                return species_info_from( jive::module::open_data(data) );
            }
            species_info   species_info_from(jive::module *);
            const species &species_from(library &lib, const species_info &info) const;

        };
    }

}

#endif

