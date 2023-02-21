
//! \file

#ifndef YACK_ASSOCIATIVE_REGISTRY_INCLUDED
#define YACK_ASSOCIATIVE_REGISTRY_INCLUDED 1

#include "yack/container/associative.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! registry of types holding their own key
    //
    //__________________________________________________________________________
    template <typename KEY, typename T>
    class registry : public associative<KEY,T>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type); //!< aliases

        //______________________________________________________________________
        //
        //! interface to insert a single type
        //______________________________________________________________________
        virtual bool insert(param_type) = 0;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~registry() noexcept {} //!< cleanup
    protected:
        inline explicit registry() noexcept {} //!< setup

        //! multipurpose display
        template <typename OSTREAM, typename NODE> static inline
        OSTREAM &display(OSTREAM &os, const NODE *node)
        {
            os << '{';
            if(node)
            {
                os << **node;
                while(NULL!=(node=node->next))
                {
                    os << ',' << **node;
                }
            }
            os << '}';
            return os;
        }
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(registry);
    };

}

#endif

