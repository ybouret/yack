
//! \file

#ifndef YACK_JIVE_PATTERN_TABLE_INCLUDED
#define YACK_JIVE_PATTERN_TABLE_INCLUDED 1

#include "yack/object.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/associative/hash/map.hpp"

namespace yack
{
    namespace jive
    {

        class pattern;

        //______________________________________________________________________
        //
        //
        //! helper to find matching patterns
        //
        //______________________________________________________________________
        struct scatter
        {
            //__________________________________________________________________
            //
            //
            //! node for scatter table
            //
            //__________________________________________________________________
            class node : public object
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit node(const pattern &) throw(); //!< setup
                node(const node &)             throw(); //!< copy
                virtual ~node()                throw(); //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                const pattern & operator*() const throw(); //!< access

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                node  *next; //!< for list
                node  *prev; //!< for list


            private:
                YACK_DISABLE_ASSIGN(node);
                const pattern &host;
            };

            //__________________________________________________________________
            //
            //
            //! list of nodes
            //
            //__________________________________________________________________
            typedef cxx_list_of<node> slot;

            //__________________________________________________________________
            //
            //
            //! code->key
            //
            //__________________________________________________________________
            class hasher
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                hasher() throw();
                ~hasher() throw();

                //______________________________________________________________
                //
                //! code to key function
                //______________________________________________________________
                size_t operator()(const uint8_t &code) const throw();

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(hasher);
            };


            //__________________________________________________________________
            //
            //
            //! base class for table
            //
            //__________________________________________________________________
            typedef hash_map<uint8_t,slot,hasher> table_;

            //__________________________________________________________________
            //
            //
            //! table of slots based on first bytes
            //
            //__________________________________________________________________
            class table : public table_
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit table() throw();
                virtual ~table() throw();

                //______________________________________________________________
                //
                //! dispatch pattern to used slots
                //______________________________________________________________
                void operator()(const pattern &);

                //______________________________________________________________
                //
                //! first node for given code, NULL meaning no corresponding
                //______________________________________________________________
                const node * operator[](const uint8_t code) const throw();

                //______________________________________________________________
                //
                //! display content
                //______________________________________________________________
                friend std::ostream & operator<<(std::ostream &, const table & );

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(table);
                void use(const uint8_t code, const pattern &);
            };

        };



#if 0
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
#endif

    }

}

#endif
