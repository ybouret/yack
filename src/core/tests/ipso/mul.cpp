#include "yack/ipso/mul.hpp"
#include "yack/utest/run.hpp"
#include "yack/math/extended.hpp"
#include "yack/math/api.hpp"
#include "yack/ordered/heap.hpp"
#include "yack/ordered/run-time-buffer.hpp"
#include "yack/ordered/compiled-buffer.hpp"
#include "yack/apex.hpp"

using namespace yack;

namespace yack
{
    namespace ipso
    {

        template <typename T>
        struct inside
        {
            typedef T type;
        };

        template <>
        struct inside<float>
        {
            typedef extended<float> type;
        };

        template <typename T>
        struct run_time_memory
        {
            typedef run_time_buffer< typename inside<T>::type, memory_type> type;
        };


        struct adding
        {

            //__________________________________________________________________
            //
            //! comparator of inside<T>
            //__________________________________________________________________
            template <typename T> struct  comparator
            {
                typedef typename inside<T>::type               inside_type;
                typedef typename scalar_for<inside_type>::type scalar_type;


                inline int operator()(const inside_type & lhs, const inside_type & rhs) const
                {
                    const scalar_type L = math::abs_of(lhs);
                    const scalar_type R = math::abs_of(rhs);
                    return (L < R) ? 1 : ((R < L) ? -1 : 0);
                }
            };

            template <typename T, typename MEM_BUFFER>
            class proto : public object_type, public heap<typename inside<T>::type,comparator<T>,MEM_BUFFER>
            {
            public:
                YACK_DECL_ARGS(T,type);
                typedef typename inside<type>::type              inside_type;
                typedef comparator<type>                         comparator_;
                typedef heap<inside_type,comparator_,MEM_BUFFER> heap_type;

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                inline explicit proto() noexcept      : object_type(), heap_type()  {} //!< setup default
                inline explicit proto(const size_t n) : object_type(), heap_type(n) {} //!< setup with possible capacity
                inline virtual ~proto() noexcept                                    {} //!< cleanup
            };
        };

        template <typename T>
        class add : public adding::proto<T, typename run_time_memory<T>::type >
        {
        public:
            YACK_DECL_ARGS(add,type);
            typedef typename inside<T>::type                             inside_type;
            typedef adding::proto<T, typename run_time_memory<T>::type > proto_class;

            inline explicit add() noexcept : proto_class() {}
            inline virtual ~add() noexcept {}
            inline explicit add(const size_t n) : proto_class(n) {}

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(add);
        };

    }

}

YACK_UTEST(ipso_mul)
{

}
YACK_UDONE()

