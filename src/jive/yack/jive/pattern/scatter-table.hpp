
//! \file

#ifndef YACK_JIVE_PATTERN_TABLE_INCLUDED
#define YACK_JIVE_PATTERN_TABLE_INCLUDED 1

#include "yack/object.hpp"
#include "yack/data/list.hpp"
#include "yack/arith/ilog2.hpp"

namespace yack
{
    namespace jive
    {

        class pattern;

        //______________________________________________________________________
        //
        //
        //! node for scatter table
        //
        //______________________________________________________________________
        class scatter_node : public object
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit scatter_node(const pattern &) throw(); //!< setup
            virtual ~scatter_node()                throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const pattern & operator*() const throw(); //!< access

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            scatter_node  *next; //!< for list
            scatter_node  *prev; //!< for list


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(scatter_node);
            const pattern &host;
        };

        //______________________________________________________________________
        //
        //
        //! slot for scatter table
        //
        //______________________________________________________________________
        typedef list_of<scatter_node> scatter_slot;


        //______________________________________________________________________
        //
        //
        //! scatter patterns according to their first bytes
        //
        //______________________________________________________________________
        class scatter_table
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef scatter_node node_type;                           //!< alias
            typedef scatter_slot slot_type;                           //!< alias
            static const size_t  slot_size = sizeof(slot_type);       //!< alias
            static const size_t  num_codes = 256;                     //!< alias
            static const size_t  work_size = num_codes * slot_size;   //!< alias
            static const size_t  work_exp2 = ilog2<work_size>::value; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit scatter_table();         //!< setup slots
            virtual ~scatter_table() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! scatter pattern from its first bytes
            void  operator()(const pattern &p); 

            //! access slot for a given code
            const slot_type & operator[](const uint8_t code) const throw();


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(scatter_table);
            slot_type *slot;
        };


    }

}

#endif
