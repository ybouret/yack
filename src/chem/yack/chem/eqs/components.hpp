
//! \file

#ifndef YACK_CHEM_COMPONENTS_INCLUDED
#define YACK_CHEM_COMPONENTS_INCLUDED 1

#include "yack/chem/eqs/component.hpp"
#include "yack/chem/eqs/actors.hpp"
#include "yack/associative/suffix/set.hpp"
#include "yack/associative/addrbook.hpp"

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

            bool contains(const Species    &) const noexcept; //!< check species is used
            bool linkedTo(const Components &) const noexcept; //!< check shared species
            void submitTo(addrbook &) const;                  //!< record all species addresses
            bool neutral() const;                             //!< check if neutral
            bool minimal() const;                             //!< check if minimal

            //! target[targetLevel] = source[soucreLevel] for each component
            template <typename TARGET, typename SOURCE> inline
            void load(TARGET &target, const IndexLevel targetLevel,
                      SOURCE &source, const IndexLevel sourceLevel) const
            {
                for(const cNode *node=(*this)->head;node;node=node->next)
                {
                    const Component &cc = ***node;
                    const Species   &sp = *cc;
                    target[ sp.indx[targetLevel] ] = source[ sp.indx[sourceLevel] ];
                }
            }

            //! assuming target is extended reals
            template <typename TARGET> inline
            void move(TARGET &target, const IndexLevel targetLevel, const Extended::Real &xi) const 
            {
                for(const cNode *node=(*this)->head;node;node=node->next)
                {
                    const Component &cc = ***node;
                    const Species   &sp = *cc;
                    target[ sp.indx[targetLevel] ] +=  xi * cc.xn;
                }
            }

            //! assuming target and source are extended reals
            template <typename TARGET, typename SOURCE> inline
            void make(TARGET &target, const IndexLevel targetLevel,
                      SOURCE &source, const IndexLevel sourceLevel,
                      const Extended::Real &xi) const
            {
                for(const cNode *node=(*this)->head;node;node=node->next)
                {
                    const Component &cc = ***node;
                    const Species   &sp = *cc;
                    target[ sp.indx[targetLevel] ] = source[ sp.indx[sourceLevel] ] + cc.xn * xi;
                }
            }

            //! display a compact list of values
            template <typename T>
            std::ostream & displayCompact(std::ostream &os, const readable<T> &field, const IndexLevel level) const
            {
                os << '{' << ' ';
                for(const cNode *cn=(*this)->head;cn;cn=cn->next)
                {
                    const Species &sp = ****cn;
                    os << sp << '=' << field[sp.indx[level]] << ' ';
                }
                os << '}';
                return os;
            }

            Extended::Real reacMassAction(Extended::Mul                  &xmul,
                                          const Extended::Real            K,
                                          const readable<Extended::Real> &C,
                                          const IndexLevel                level) const;

            Extended::Real prodMassAction(Extended::Mul                  &xmul,
                                          const readable<Extended::Real> &C,
                                          const IndexLevel                level) const;

            Extended::Real massAction(Extended::Mul                  &xmul,
                                      const Extended::Real            K,
                                      const readable<Extended::Real> &C,
                                      const IndexLevel                level) const;


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

