

//! \file

#ifndef YACK_DATA_SMALL_BANK_INCLUDED
#define YACK_DATA_SMALL_BANK_INCLUDED 1

#include "yack/data/small/list.hpp"
#include "yack/data/pool/cxx.hpp"
#include "yack/counted.hpp"
#include "yack/ptr/arc.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //!
    //
    //__________________________________________________________________________
    template <typename T>
    class small_bank : public object, public counted, public cxx_pool_of< small_node<T> >
    {
    public:
        YACK_DECL_ARGS(T,type);                   //!< aliases
        typedef arc_ptr<small_bank>    pointer;   //!< alias
        typedef small_node<type>       node_type; //!< alias
        typedef cxx_pool_of<node_type> self_type; //!< alias

        inline explicit small_bank() throw() : object(), counted(), self_type()  {}
        inline virtual ~small_bank() throw() {}

        inline void collect(small_list<type> &debt) throw() {
            while(debt.size) store(debt.pop_back());
        }



    private:
        YACK_DISABLE_COPY_AND_ASSIGN(small_bank);
    };

}

#endif
