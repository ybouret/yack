
//! \file

#ifndef YACK_CHEMICAL_LIBRARY_INCLUDED
#define YACK_CHEMICAL_LIBRARY_INCLUDED 1

#include "yack/chem/species.hpp"
#include "yack/chem/gathering.hpp"
#include "yack/associative/suffix/set.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        // types for library
        //
        //______________________________________________________________________
        typedef suffix_set<string,const species::pointer> species_set; //!< alias
        typedef species_set::knot_type                    snode;       //!< alias


        //______________________________________________________________________
        //
        //
        //! library of unique species
        //
        //______________________________________________________________________
        class library : public gathering, public readable<const species>
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const char clid[]; //!< chemical::library

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit library() throw();    //!< setup
            virtual ~library() throw();    //!< cleanup
            library(const library &other); //!< hard copy

            //__________________________________________________________________
            //
            // readable interface
            //__________________________________________________________________
            virtual size_t size() const throw();                           //!< number of species
            virtual const_type & operator[](const size_t i) const throw(); //!< slow access

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const   snode *head() const throw(); //!< sequential access

            //! manual creation of a NEW species
            template <typename NAME> inline
            const species & operator()(const NAME &name, const int z)
            {
                return use( new species(name,z,size()+1) );
            }


            const species &operator()(const string &expr);        //!< on the fly parsing of new/existing species
            const species &operator()(const char   *expr);        //!< on the fly parsing of new/existing species
            const species *look_up(const string &) const throw(); //!< no-throw look up a species by name
            const species & operator[](const string &) const;     //!< access existing species by name
            const species & operator[](const char   *) const;     //!< access existing species by name

            //! display species+info
            friend std::ostream & operator<<(std::ostream &os, const library &lib);



        private:
            YACK_DISABLE_ASSIGN(library);
            species_set sdb;
            const species & use(species *s);

        };



    }

}

#endif
