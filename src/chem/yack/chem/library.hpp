
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
        typedef suffix_set<string,const species::pointer> species_set;
        typedef typename species_set::knot_type           snode;

        class library : public compendium
        {
        public:
            explicit library() throw();
            virtual ~library() throw();

            const snode *head() const throw();
            size_t       size() const throw();

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
            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(library);
            species_set db;
        };

    }

}

#endif
