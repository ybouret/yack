
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
        typedef typename species_set::knot_type           snode;       //!< alias

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
            const snode *head() const throw(); //!< first species
            size_t       size() const throw(); //!< number of species

            //! display named values
            template <typename T> inline
            void operator()(std::ostream &os, const readable<T> &arr)
            {
                os << '{' << std::endl;
                for(const snode *node=head();node;node=node->next)
                {
                    const species &s = ***node;
                    pad(os<< ' ' << '[' << s.name << ']',s) << " = " << s[arr] << std::endl;
                }
                os << '}' << std::endl;
            }

            //! declare/recall species
            const species &operator()(const string &name, const int z);

            //! declare/recall species
            const species &operator()(const char   *name, const int z);

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
