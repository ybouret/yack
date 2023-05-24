
//! \file

#ifndef YACK_CHEM_LIBRARY_INCLUDED
#define YACK_CHEM_LIBRARY_INCLUDED 1

#include "yack/chem/species.hpp"
#include "yack/associative/suffix/set.hpp"

namespace yack
{
    namespace Chemical
    {
        typedef suffix_set<string,Species::Pointer> SpeciesDB;
        typedef SpeciesDB::knot_type                SNode;
        typedef list_of<SNode>                      SList;

        class Library : public Gathering
        {
        public:
            static const char CLID[];

            explicit Library() noexcept;
            virtual ~Library() noexcept;

            const SList * operator->() const throw();

            template <typename NAME>
            const Species & operator()(const NAME &id,
                                       const int   zz)
            {
                return inserted( new Species(id,zz) );
            }

            const Species * query(const string &) noexcept;
            const Species * query(const char   *);
            

        private:
            YACK_DISABLE_ASSIGN(Library);
            SpeciesDB sdb;

            const Species & inserted(Species *sp);
        };
    }

}

#endif

