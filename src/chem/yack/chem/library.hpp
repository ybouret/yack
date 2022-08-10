
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

        typedef suffix_set<string,const species::pointer> species_set;
        typedef species_set::knot_type                    snode;

        class library :public gathering, public readable<const species>
        {
        public:
            static const char clid[];

            explicit library() throw();
            virtual ~library() throw();
            library(const library &other); //!< hard copy

            const   snode *head() const throw(); //!< sequential access
            virtual size_t size() const throw(); //!< number of species
            virtual const_type & operator[](const size_t i) const throw(); //!< slow access

            template <typename NAME> inline
            const species & operator()(const NAME &name, const int z)
            {
                return use( new species(name,z,size()+1) );
            }

            const species *look_up(const string &) const throw();

            const species & operator[](const string &) const;
            const species & operator[](const char   *) const;
            
            friend std::ostream & operator<<(std::ostream &os, const library &lib);



        private:
            YACK_DISABLE_ASSIGN(library);
            species_set sdb;
            const species & use(species *s);

        };



    }

}

#endif
