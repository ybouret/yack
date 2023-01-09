
//! \file

#ifndef YACK_DINKY_REGISTRY_INCLUDED
#define YACK_DINKY_REGISTRY_INCLUDED 1

#include "yack/data/dinky/node.hpp"
#include "yack/data/dinky/root.hpp"
#include "yack/data/list/sort.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! registry of type
    //
    //__________________________________________________________________________
    template <typename T, template <typename> class ZPOOL>
    class  dinky_registry :
    public dinky_root<dinky_node<T>, typename ZPOOL< dinky_node<T> >::proxy >
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef dinky_node<T>                    node_type;  //!< alias
        typedef ZPOOL<node_type>                 zpool_type; //!< alias
        typedef typename zpool_type::proxy       proxy_type; //!< alias
        typedef dinky_root<node_type,proxy_type> root_type;  //!< alias
        typedef typename root_type::list_type    list_type;  //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit dinky_registry() throw() : root_type()                           {} //!< setup
        inline explicit dinky_registry(const proxy_type &user) throw() : root_type(user) {} //!< setup with cache
        inline virtual ~dinky_registry() throw()                                         {} //!< cleanup
        inline          dinky_registry(const dinky_registry &_) : root_type(_)           {} //!< copy

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
      
        //! push back
        template <typename U> inline
        dinky_registry & operator<<(const U &u) {
            this->annex(u);
            return *this;
        }
        
        //! push front
        template <typename U> inline
        dinky_registry & operator>>(const U &u) {
            this->shove(u);
            return *this;
        }
        
        //! pull head value
        inline T pull_head() { assert(this->head);
            T res = **(this->head); this->cut_head(); return res;
        }
        
        //! pull tail value
        inline T pull_tail() { assert(this->tail);
            T res = **(this->tail); this->cut_tail(); return res;
        }
        
        //! sort
        template <typename FUNC> inline
        void sort_with( FUNC &func )
        {
            kernel::primary_sort_callback<node_type,FUNC> cb = { func };
            merge_list_of<node_type>::sort(*this,cb);
        }
        
    private:
        YACK_DISABLE_ASSIGN(dinky_registry);
    };
    
}

#endif

