//! \file

#ifndef YACK_CHEMICAL_SP_LIST_INCLUDED
#define YACK_CHEMICAL_SP_LIST_INCLUDED 1

#include "yack/chem/species.hpp"
#include "yack/data/list/meta.hpp"
#include "yack/associative/lexicon.hpp"

namespace yack {

    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        // base types for list of existing species
        //
        //______________________________________________________________________
        typedef meta_list<const species> sp_list_;    //!< list of species, base class
        typedef sp_list_::node_type      sp_node;     //!< node within sp_list
        typedef lexicon<size_t>          sp_book;     //!< alias
        class library;
        
        //______________________________________________________________________
        //
        //
        //! list of existing species, with sorting capabilities
        //
        //______________________________________________________________________
        class sp_list : public sp_list_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit sp_list() throw();              //!< setup
            virtual ~sp_list() throw();              //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void     sort()    throw();                      //!< sort by increasing index
            void     load(const sp_book &, const library &); //!< load and sort
            void     shed(const sp_list &) throw();          //!< remove from self
            
            //! compare by increasing indices
            static int node_compare(const sp_node *, const sp_node *) throw();

            //! transfer only active part
            template <typename TARGET, typename SOURCE> inline
            void transfer(TARGET &target, SOURCE &source) const
            {
                for(const sp_node *node=this->head;node;node=node->next)
                {
                    const size_t j = ***node;
                    target[j] = source[j];
                }
            }


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(sp_list);
        };

    }

}

#endif

