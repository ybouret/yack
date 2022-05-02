//! \file

#ifndef YACK_SYNC_TOPOLOGY_INCLUDED
#define YACK_SYNC_TOPOLOGY_INCLUDED 1

#include "yack/string/fwd.hpp"
#include "yack/data/small/list.hpp"
#include "yack/counted.hpp"
#include "yack/ptr/arc.hpp"
#include <iostream>

namespace yack
{

    namespace concurrent
    {

        namespace quark
        {
            //__________________________________________________________________
            //
            //
            // definitions
            //
            //__________________________________________________________________
            typedef small_list<size_t>     ulist_type; //!< alias
            typedef ulist_type::node_type  unode_type; //!< alias

            //__________________________________________________________________
            //
            //
            //! list of cpus to use
            //
            //__________________________________________________________________
            class topology : public object, public counted, public ulist_type
            {
            public:
                ~topology() throw();      //!< cleanup
                topology(const size_t n); //!< n procs
                topology(const string &); //!< parse content
                topology(const char   *); //!< parse content
                topology();               //!< default or parse existing YACK_NUM_THREADS

                //! display
                friend std::ostream & operator<<(std::ostream &, const topology &);


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(topology);
                void       linear(const size_t n);
                void       expand(const string &);
            };
        }

        //______________________________________________________________________
        //
        //
        //! alias
        //
        //______________________________________________________________________
        typedef arc_ptr<const quark::topology> topology_;

        //______________________________________________________________________
        //
        //
        //! high level topology
        //
        //______________________________________________________________________
        class topology : public topology_
        {
        public:
            virtual ~topology() throw();       //!< cleanup
            explicit topology();               //!< default or parse existing YACK_NUM_THREADS
            explicit topology(const size_t  ); //!< n procs
            explicit topology(const string &); //!< parse content
            explicit topology(const char   *); //!< parse content

        private:
            YACK_DISABLE_ASSIGN(topology);
        };



    }

}

#endif

