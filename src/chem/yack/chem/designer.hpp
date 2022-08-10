
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

        namespace nucleus
        {
            //__________________________________________________________________
            //
            //
            //! info for species identification
            //
            //__________________________________________________________________
            class sp_info
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                sp_info();                //!< setup empty/0
                sp_info(const sp_info &); //!< copy
                ~sp_info() throw();      //!< cleanup

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                string name; //!< species name
                int    z;    //!< species charge

            private:
                YACK_DISABLE_ASSIGN(sp_info);
            };
        }

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

            //__________________________________________________________________
            //
            // internal methods
            //__________________________________________________________________

            //! parse string/text into info
            template <typename DATA> inline
            nucleus::sp_info get_species_info(const DATA &data) {
                return species_info_from( jive::module::open_data(data) );
            }

            nucleus::sp_info   species_info_from(jive::module *);                       //!< parse from module
            const species     &species_from(library &, const nucleus::sp_info &) const; //!< search/create for library

        };
    }

}

#endif

