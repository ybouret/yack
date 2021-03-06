//! \file

#ifndef YACK_DATA_KNOT_INCLUDED
#define YACK_DATA_KNOT_INCLUDED 1

#include "yack/object.hpp"
#include "yack/arith/align.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/data/pool/cxx.hpp"
#include "yack/type/args.hpp"

namespace yack
{
    namespace kernel
    {
        //______________________________________________________________________
        //
        //
        //! knot to store data for tree/table
        //
        //______________________________________________________________________
        template <typename T,typename NODE>
        class data_knot : public object
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            YACK_DECL_ARGS(T,type);                    //!< aliases
            typedef cxx_list_of<data_knot> list_type;  //!< alias
            typedef cxx_pool_of<data_knot> pool_type;  //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline explicit data_knot() throw() : next(0), prev(0), node(0), data(0), impl() {} //!< setup
            inline virtual ~data_knot() throw() { if(data) free(); }                            //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! construct data
            inline void make(const_type &args)
            {
                assert(NULL==data);
                data = new ( out_of_reach::zset(impl,sizeof(impl)) ) mutable_type(args);
            }

            //! destruct data
            inline data_knot *free() throw()
            {
                assert(NULL!=data);
                data->~mutable_type();
                data = NULL;
                node = NULL;
                return this;
            }

            inline bool         is_alive()  const throw() { return NULL != data; }              //!< check satus
            inline type       & operator*()       throw() { assert(is_alive()); return *data; } //!< access data
            inline const_type & operator*() const throw() { assert(is_alive()); return *data; } //!< access const data

            inline type       * operator->()       throw() { assert(is_alive()); return data; } //!< drill-down
            inline const_type * operator->() const throw() { assert(is_alive()); return data; } //!< drill-down

            //! forward display to node
            template <typename OSTREAM>
            inline void disp(OSTREAM &os) const
            {
                assert(node);
                node->disp(os);
            }

            //! forward encoding to node
            inline void encode( typename NODE::path_type &path ) const
            {
                assert(is_alive());
                node->encode(path);
            }


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            data_knot    *next; //!< for list/pool
            data_knot    *prev; //!< for list
            NODE         *node; //!< node within structure

        private:
            mutable_type  *data;
            void          *impl[ YACK_WORDS_FOR(T) ];
            YACK_DISABLE_COPY_AND_ASSIGN(data_knot);
        };
    }

}

#endif
