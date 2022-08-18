
//! \file

#ifndef YACK_CHEMICAL_ACTIVE_INCLUDED
#define YACK_CHEMICAL_ACTIVE_INCLUDED 1

#include "yack/chem/library.hpp"
#include "yack/data/list/meta.hpp"

namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        // types for list of active species
        //
        //______________________________________________________________________
        typedef meta_list<const species> alist; //!< alias
        typedef alist::node_type         anode; //!< alias

        //______________________________________________________________________
        //
        //
        //! list of active species
        //
        //______________________________________________________________________
        class active_list : public alist
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit active_list() throw();      //!< setup empty
            virtual ~active_list() throw();      //!< cleanup
            explicit active_list(const snode *); //!< build from species list

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! transfer only active part
            template <typename TARGET, typename SOURCE> inline
            void transfer(TARGET &target, SOURCE &source) const
            {
                for(const anode *node=this->head;node;node=node->next)
                {
                    const size_t j = ***node;
                    target[j] = source[j];
                }
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(active_list);
        };
        
    }

}

#endif
