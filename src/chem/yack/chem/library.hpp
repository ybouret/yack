
//! \file

#ifndef YACK_CHEM_LIBRARY_INCLUDED
#define YACK_CHEM_LIBRARY_INCLUDED 1

#include "yack/chem/species.hpp"
#include "yack/associative/suffix/set.hpp"
#include "yack/randomized/bits.hpp"

namespace yack
{
    namespace Chemical
    {
        typedef suffix_set<string,Species::Pointer> SpecDB; //!< alias
        typedef SpecDB::knot_type                   sNode;     //!< alias
        typedef list_of<sNode>                      sList;     //!< alias

        //______________________________________________________________________
        //
        //
        //! constant equilibrium
        //
        //______________________________________________________________________
        class Library : public Gathering
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const char CLID[]; //!< "Chemical::Libary"
            static const int  pmin = -30;
            static const int  pmax =  1;
            static double     Conc(randomized::bits &, const double probaNeg) noexcept;
            static void       Conc(writable<double> &, randomized::bits &, const double probaNeg) noexcept;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Library() noexcept; //!< setup
            virtual ~Library() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const sList * operator->() const throw(); //!< access

            //! formatted output
            friend std::ostream & operator<<(std::ostream &, const Library &);
            
            //! insert a new/multiple species
            template <typename NAME>
            const Species & operator()(const NAME &id,
                                       const int   zz)
            {
                return inserted( new Species(id,zz) );
            }

            const Species * query(const string &) noexcept; //!< query by name
            const Species * query(const char   *);          //!< query by name

            template <typename ARRAY> inline
            std::ostream & operator()(std::ostream &os, const char *pfx, ARRAY &arr, const char *sfx) const
            {
                if(!pfx) pfx = "";
                if(!sfx) sfx = "";
                os << '{' << std::endl;
                for(const sNode *node=(*this)->head;node;node=node->next)
                {
                    const Species &sp = ***node;
                    pad(os << ' ' << pfx << sp.name << sfx,sp) << " = " << arr[ sp.indx[TopLevel] ] << std::endl;
                }
                os << '}';
                return os;
            }


        private:
            YACK_DISABLE_ASSIGN(Library);
            SpecDB sdb;

            const Species & inserted(Species *sp);
        };
    }

}

#endif

