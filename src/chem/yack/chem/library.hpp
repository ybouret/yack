
//! \file

#ifndef YACK_CHEM_LIBRARY_INCLUDED
#define YACK_CHEM_LIBRARY_INCLUDED 1

#include "yack/chem/compendium.hpp"
#include "yack/chem/species.hpp"
#include "yack/associative/suffix/set.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        // definitions
        //______________________________________________________________________
        typedef suffix_set<string,const species::pointer> species_set; //!< alias
        typedef species_set::knot_type                    snode;       //!< alias

        //______________________________________________________________________
        //
        //
        //!set of unique species
        //
        //______________________________________________________________________
        class library : public compendium
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char clid[]; //!< library


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            explicit library() throw(); //!< setup
            virtual ~library() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const snode   *head()   const throw();  //!< first species
            virtual size_t size()   const throw();  //!< number of species
            size_t         active() const throw();  //!< active species
            size_t         primary() const throw(); //!< primary species

            //! display named values
            template <typename ARR> inline
            void operator()(std::ostream &os, ARR &arr, const char *pfx = NULL) const
            {
                if(!pfx) pfx="";
                os << '{';
                for(const snode *node=head();node;node=node->next)
                {
                    const species &s = ***node;
                    os << std::endl;
                    pad(os<< pfx << ' ' << '[' << s.name << ']',s) << " = " << arr[*s];
                }
                os << ' ' << '}' << std::endl;
            }

            //! declare/recall species
            const species &operator()(const string &name, const int z);

            //! declare/recall species
            const species &operator()(const char   *name, const int z);

            //! SLOW access species by index
            const species &operator[](const size_t indx) const throw();

            //! parse single species
            const species &parse(const string &);

            //! parse single species
            const species &parse(const char   *);

            //! fill C
            void  fill(writable<double> &C,
                       const double      zero_above,
                       randomized::bits &ran) throw();

            //! display info
            friend std::ostream & operator<<(std::ostream &, const library &);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(library);
            species_set db;

        };

    }

}

#endif
