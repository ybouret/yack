//! \file

#ifndef YACK_CHEMICAL_EQ_TEAM_INCLUDED
#define YACK_CHEMICAL_EQ_TEAM_INCLUDED 1

#include "yack/chem/equilibrium.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    namespace chemical
    {
        typedef meta_list<const equilibrium> eq_team_; //!< alias
        typedef meta_node<const equilibrium> eq_node;  //!< alias

        //______________________________________________________________________
        //
        //
        //! team of equilibria
        //
        //______________________________________________________________________
        class eq_team : public object, public eq_team_
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef auto_ptr<eq_team> pointer; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit eq_team() throw(); //!< setup
            virtual ~eq_team() throw(); //!< cleanup
            eq_team(const eq_team &);   //!< copy

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! display as list
            friend std::ostream & operator<<(std::ostream &os, const eq_team &self)
            {
                const eq_node *node = self.head;
                if(node)
                {
                    os << "{ " << (**node).name;
                    for(node=node->next;node;node=node->next)
                    {
                        os << ", " << (**node).name;
                    }
                    os << " }";
                    return os;
                }
                else
                {
                    return os << "{}";
                }
            }

            //! compare by index
            static int compare(const eq_node *, const eq_node *) throw();

            //! sort using compare
            void sort() throw();

        private:
            YACK_DISABLE_ASSIGN(eq_team);
        };

    }

}

#endif

