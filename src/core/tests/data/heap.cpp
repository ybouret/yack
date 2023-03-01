#include "yack/ordered/prio-queue.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"

#include "yack/memory/allocator/pooled.hpp"
#include "yack/memory/allocator/dyadic.hpp"

#include "yack/container.hpp"
#include "yack/container/as-copy.hpp"


using namespace yack;

namespace
{
    struct icompare
    {
        inline int operator()(const int lhs, const int rhs) noexcept {
            return lhs-rhs;
        }
    };

    struct scompare
    {
        inline int operator()(const string &lhs, const string &rhs) noexcept
        {
            return string::compare(lhs,rhs);
        }
    };

    template <typename T, const size_t N>
    class fixed_buffer
    {
    public:
        static const size_t type_size = sizeof(T);
        static const size_t wksp_size = type_size * N;
        static const bool   flexible  = false;

        inline fixed_buffer() noexcept : workspace(0), wksp() { on(); }

        inline fixed_buffer(const size_t n) :
        workspace(0),
        wksp()
        {
            if(n>num_items) throw exception("fixed_buffer overflow");
            on();
        }



        inline ~fixed_buffer() noexcept
        {
            workspace = 0;
        }

        T                  *workspace;
        static const size_t num_items = N;

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(fixed_buffer);
        void *wksp[ YACK_WORDS_GEQ(wksp_size) ];

        inline void on() noexcept {
            YACK_STATIC_ZSET(wksp);
            workspace = coerce_cast<T>(&wksp[0]);
        }


    };


    template <typename T, typename ALLOCATOR>
    class alloc_buffer
    {
    public:
        static const bool   flexible  = true;

        inline alloc_buffer() noexcept :
        workspace(0),
        num_items(0),
        num_bytes(0)
        {
        }

        inline alloc_buffer(const size_t n) :
        workspace(0),
        num_items(n),
        num_bytes(0)
        {
            static memory::allocator &mgr = ALLOCATOR::instance();
            workspace = mgr.allocate<T>(num_items,num_bytes);
        }



        inline ~alloc_buffer() noexcept
        {
            if(workspace) {
                static memory::allocator &mgr = ALLOCATOR::location();
                mgr.withdraw(workspace,num_bytes);
                num_items = 0;
            }
        }

        T     *workspace;
        size_t num_items;
        size_t num_bytes;

        inline void swap_with( alloc_buffer &other ) noexcept
        {
            cswap(workspace,other.workspace);
            cswap(num_items,other.num_items);
            cswap(num_bytes,other.num_bytes);
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(alloc_buffer);
    };




    static const char * const heap_category = "heap";

    template <
    typename T,
    typename COMPARATOR,
    typename MEM_BUFFER>
    class heap : public container
    {
    public:
        YACK_DECL_ARGS(T,type);
        typedef prio_queue<T,COMPARATOR>      pqueue_type;
        typedef MEM_BUFFER                    buffer_type;

        inline explicit heap() noexcept : M(), Q(M.workspace,M.num_items) {}

        inline explicit heap(const size_t n) : M(n), Q(M.workspace,M.num_items) {}

        inline virtual ~heap() noexcept {}

        template <typename OTH_BUFFER>
        inline heap(const heap<T,COMPARATOR,OTH_BUFFER> &other, const as_copy_t &) :
        M(other.size()), Q(M.workspace,M.num_items,other.getQ())
        {
            
        }


        inline void push(param_type args)
        {
            Q.insert(args);
        }

        virtual inline void        free()            noexcept { Q.finish(); }
        virtual inline size_t      size()      const noexcept { return Q.count; }
        virtual inline size_t      capacity()  const noexcept { return Q.total; }
        virtual inline size_t      available() const noexcept { return Q.total - Q.count; }
        virtual inline const char *category()  const noexcept { return heap_category; }
        virtual inline void        reserve(size_t n)
        {
            static const int2type<MEM_BUFFER::flexible> behavior = {};
            reserve(behavior,n);
        }

        virtual inline void        release() noexcept
        {
            static const int2type<MEM_BUFFER::flexible> behavior = {};
            release(behavior);
        }

        inline const pqueue_type & getQ() const noexcept { return Q; }

    private:
        buffer_type M;
        pqueue_type Q;

        YACK_DISABLE_COPY_AND_ASSIGN(heap);

        inline void release( const int2type<false> & ) noexcept { Q.finish(); }
        inline void reserve( const int2type<false> &, size_t n)
        {
            throw exception("no possible reserve(%u)", unsigned(n));
        }

        inline void release( const int2type<true>  & ) noexcept {
            Q.finish();
            if(Q.total>0)
            {
                buffer_type M0;
                pqueue_type Q0(M0.workspace,M0.num_items);
                M0.swap_with(M);
                Q0.swap_with(Q);
            }
        }

        inline void reserve(const int2type<true> &, size_t n)
        {
            if(n>0)
            {
                const size_t new_capacity = Q.total + n;
                buffer_type  m(new_capacity);
                pqueue_type  q(m.workspace,m.num_items); assert(q.total>Q.total);
                out_of_reach::swap(q.tree,Q.tree,(q.count=Q.count)*sizeof(type));
                Q.count = 0;
                m.swap_with(M);
                q.swap_with(Q);
            }
        }


    };


}

YACK_UTEST(data_heap)
{
    randomized::rand_ ran;


    heap< int,icompare,fixed_buffer<int,5> >              fih;
    heap< int,icompare,alloc_buffer<int,memory::pooled> > dih1;
    heap< int,icompare,alloc_buffer<int,memory::dyadic> > dih2(7);

    std::cerr << "fih:  " << fih.size()  << " / " << fih.capacity()  << std::endl;
    std::cerr << "dih1: " << dih1.size() << " / " << dih1.capacity() << std::endl;
    std::cerr << "dih2: " << dih2.size() << " / " << dih2.capacity() << std::endl;
    std::cerr << std::endl;

    fih.release();
    dih1.release();
    dih2.release();

    std::cerr << "fih:  " << fih.size()  << " / " << fih.capacity()  << std::endl;
    std::cerr << "dih1: " << dih1.size() << " / " << dih1.capacity() << std::endl;
    std::cerr << "dih2: " << dih2.size() << " / " << dih2.capacity() << std::endl;
    std::cerr << std::endl;

    dih1.reserve(4);
    dih2.reserve(4);
    std::cerr << "dih1: " << dih1.size() << " / " << dih1.capacity() << std::endl;
    std::cerr << "dih2: " << dih2.size() << " / " << dih2.capacity() << std::endl;
    std::cerr << std::endl;




    fih.push(1);

    {
        heap< int,icompare,fixed_buffer<int,5> >              fih_copy1(fih,as_copy);
        heap< int,icompare,alloc_buffer<int,memory::pooled> > fih_copy2(fih,as_copy);
        heap< int,icompare,alloc_buffer<int,memory::dyadic> > fih_copy3(fih,as_copy);

    }






}
YACK_UDONE()
