
//! \file

#ifndef YACK_CHEM_COMPONENTS_INCLUDED
#define YACK_CHEM_COMPONENTS_INCLUDED 1

#include "yack/chem/eqs/component.hpp"
#include "yack/chem/eqs/actors.hpp"
#include "yack/associative/suffix/set.hpp"

namespace yack
{
    namespace Chemical
    {
        typedef suffix_set<string,Component::Pointer> ComponentsDB;
        typedef ComponentsDB::knot_type               cNode;
        typedef list_of<cNode>                        cList;

        class Components
        {
        public:
            static const char CLID[];
            
            explicit Components() noexcept;
            virtual ~Components() noexcept;

            const cList * operator->() const throw();

            void operator()(const int, const Species &);

            std::ostream & display(std::ostream &) const;

            template <typename T> inline
            void fill( writable<T> &Nu, const IndexLevel level) const
            {
                for(const cNode *node= (*this)->head; node; node=node->next)
                {
                    const Component &cc = ***node;
                    const Species   &sp = *cc;
                    Nu[ sp.indx[level] ] = cc.nu;
                }
            }
            
            const Actors reac;
            const Actors prod;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Components);
            ComponentsDB cdb;
        };

    }

}

#endif

