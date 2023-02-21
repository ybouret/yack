//! \file

#ifndef YACK_SYNC_TOPOLOGY_INCLUDED
#define YACK_SYNC_TOPOLOGY_INCLUDED 1

#include "yack/string/fwd.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/object.hpp"
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
            //! placement node
            //
            //__________________________________________________________________
            class unode_type : public object
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit unode_type(const size_t which) noexcept; //!< setup core
                virtual ~unode_type() noexcept;                   //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                size_t operator*() const noexcept; //!< return core number

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                unode_type  *next; //!< for list
                unode_type  *prev; //!< for list
                const size_t core; //!< cpu rank
                const size_t rank; //!< global rank 0:num_threads-1
                const size_t indx; //!< global indx 1:num_threads

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(unode_type);
            };

            //__________________________________________________________________
            //
            //
            //! placement nodes
            //
            //__________________________________________________________________
            typedef cxx_list_of<unode_type> ulist_type;
            
            

            //__________________________________________________________________
            //
            //
            //! list of cpus to use
            //
            //__________________________________________________________________
            class topology : public object, public counted, public ulist_type
            {
            public:
                ~topology() noexcept;      //!< cleanup
                topology(const size_t n); //!< n procs
                topology(const string &); //!< parse content
                topology(const char   *); //!< parse content
                topology();               //!< default or parse existing YACK_NUM_THREADS

                //! display
                friend std::ostream & operator<<(std::ostream &, const topology &);


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(topology);
                void       linear(const size_t n); //!< n linear threads
                void       expand(const string &); //!< parser
                void       update() noexcept;       //!< post process rank/indx
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
            virtual ~topology() noexcept;       //!< cleanup
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

