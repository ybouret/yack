//! \file

#ifndef YACK_IPSO_ADDING_INCLUDED
#define YACK_IPSO_ADDING_INCLUDED 1

#include "yack/ipso/api.hpp"
#include "yack/math/api.hpp"
#include "yack/ordered/heap.hpp"
#include "yack/large-object.hpp"

namespace yack
{
    namespace ipso
    {
        
        struct adding
        {

            //__________________________________________________________________
            //
            //! comparator of inside<T>
            //__________________________________________________________________
            template <typename T> struct  comparator
            {
                typedef typename inside<T>::type               inside_type; //!< alias
                typedef typename scalar_for<inside_type>::type scalar_type; //!< alias


                //! compare inside types
                inline int operator()(const inside_type & lhs, const inside_type & rhs) const
                {
                    const scalar_type L = math::abs_of(lhs);
                    const scalar_type R = math::abs_of(rhs);
                    return (L < R) ? 1 : ((R < L) ? -1 : 0);
                }
            };

            //__________________________________________________________________
            //
            //
            //! adding prototype, using heap for memory
            //
            //__________________________________________________________________
            template <typename T, typename MEM_BUFFER>
            class proto : public large_object, public counted, public heap<typename inside<T>::type,comparator<T>,MEM_BUFFER>,  public api<T>
            {
            public:
                //______________________________________________________________
                //
                // definitions
                //______________________________________________________________
                YACK_DECL_ARGS(T,type);                                       //!< aliases
                typedef typename inside<type>::type              inside_type; //!< alias
                typedef comparator<type>                         comparator_; //!< alias
                typedef heap<inside_type,comparator_,MEM_BUFFER> heap_type;   //!< alias

                using  heap_type::push;
                using  heap_type::size;
                using  heap_type::pull;
                using  heap_type::grow;

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                inline explicit proto() noexcept      : heap_type(),  api<T>(static_cast<container&>(*this)) {} //!< setup default
                inline explicit proto(const size_t n) : heap_type(n), api<T>(static_cast<container&>(*this)) {} //!< setup with possible capacity
                inline virtual ~proto() noexcept                                                             {} //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! append value to heap
                inline virtual void append(const inside_type &args) {
                    push(args);
                }

                //! append precompiled values to heap
                template <typename ARRAY> inline
                void load(ARRAY &arr) {
                    for(size_t i=arr.size();i>0;--i)
                        push( arr[i] );
                }

                //! get the current sum
                inline virtual inside_type reduce() {
                    switch(size())
                    {
                        case 0:  return 0;
                        case 1:  return pull();
                        default: break;
                    }
                    assert(size() > 1);
                REDUCE:
                    const inside_type lhs = pull(); assert(size() >= 1);
                    const inside_type rhs = pull();
                    const inside_type tmp = lhs + rhs;
                    if (size() <= 0) return tmp;
                    grow(tmp);
                    goto REDUCE;
                }

                //! load simple product
                template <typename LHS, typename RHS>
                void prod(typename type_traits<LHS>::parameter_type lhs,
                          typename type_traits<RHS>::parameter_type rhs)
                {
                    const inside_type l = inside<T>::send( lhs );
                    const inside_type r = inside<T>::send( rhs );
                    const inside_type p = l*r;
                    push(p);
                }

                //! load dot product
                template <typename LHS, typename RHS> inline
                proto &dot( LHS &lhs, RHS &rhs )
                {
                    assert(lhs.size()==rhs.size());
                    const size_t n = lhs.size();
                    this->resume(n);
                    for(size_t i=n;i>0;--i)
                    {
                        const inside_type l = inside<T>::send( lhs[i] );
                        const inside_type r = inside<T>::send( rhs[i] );
                        const inside_type p = l*r;
                        push(p);
                    }
                    return *this;
                }

                



            private:
                YACK_DISABLE_COPY_AND_ASSIGN(proto);
            };
        };
    }

}

#endif

