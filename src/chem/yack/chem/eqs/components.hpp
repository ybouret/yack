
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
        typedef suffix_set<string,Component::Pointer> ComponentsDB; //!< alias
        typedef ComponentsDB::knot_type               cNode;        //!< alias
        typedef list_of<cNode>                        cList;        //!< alias

        //______________________________________________________________________
        //
        //
        //! Components hold a database of components and lists of reac/prod
        //
        //______________________________________________________________________
        class Components
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const char CLID[]; //!< "Chemical::Components"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Components() noexcept;
            virtual ~Components() noexcept;

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const cList *  operator->() const throw();              //!< access
            void           operator()(const int, const Species &);  //!< register nu * species
            std::ostream & display(std::ostream &) const;           //!< output reac <=> prod

            //! fill array using indx[level] of species
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

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Actors reac; //!< reactants list
            const Actors prod; //!< products  list
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Components);
            ComponentsDB cdb;
        };

    }

}

#endif

