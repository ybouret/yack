//! \file

#ifndef YACK_CHEMICAL_DESIGNER_INCLUDED
#define YACK_CHEMICAL_DESIGNER_INCLUDED 1

#include "yack/chem/library.hpp"
#include "yack/chem/components.hpp"
#include "yack/chem/designer/cm-info.hpp"

#include "yack/singleton.hpp"

#include "yack/jive/parser.hpp"
#include "yack/jive/syntax/translator.hpp"

namespace yack
{
    namespace chemical
    {


        //______________________________________________________________________
        //
        //
        //! singleton to design systems
        //
        //______________________________________________________________________
        class designer : public singleton<designer>
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            typedef jive::parser              parser; //!< alias
            typedef jive::syntax::translator  linker; //!< alias

            //__________________________________________________________________
            //
            // singleton parameters
            //__________________________________________________________________
            static const char * const       call_sign;      //!< "chemical::designer"
            static const at_exit::longevity life_time = 90; //!< before jive::characters


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! parse data and check/get/create corresponding species
            template <typename DATA> inline
            const species & operator()(library &lib, const DATA &data)
            {
                const  nucleus::sp_info info = get_species_info(data);
                return species_from(lib,info);
            }

            //! parse data to fill components
            template <typename DATA> inline
            void operator()(components &cmp,
                            library    &lib,
                            const DATA &data)
            {
                nucleus::cm_infos cmi;
                cm_infos_from(jive::module::open_data(data),cmi);
                components_to(cmp,lib,cmi);
            }




        private:
            YACK_DISABLE_COPY_AND_ASSIGN(designer);
            friend class singleton<designer>;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit designer();
            virtual ~designer() throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            auto_ptr<parser> spp; //!< species parser
            auto_ptr<linker> spl; //!< species linker

            auto_ptr<parser> cmp; //!< components parser
            auto_ptr<linker> cml; //!< components linker

            //__________________________________________________________________
            //
            // internal methods
            //__________________________________________________________________

            //! parse string/text into info
            template <typename DATA> inline
            nucleus::sp_info get_species_info(const DATA &data)
            {
                return sp_info_from( jive::module::open_data(data) );
            }

            nucleus::sp_info   sp_info_from(jive::module *);                            //!< parse from module
            const species     &species_from(library &, const nucleus::sp_info &) const; //!< search/create for library


            void cm_infos_from(jive::module *, nucleus::cm_infos &);                //!< parse cm_infos from module
            void components_to(components &, library &, const nucleus::cm_infos &); //!< fill components/library

        };
    }

}

#endif

