#include "yack/ordered/prio-queue.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"

#include "yack/memory/allocator/pooled.hpp"
#include "yack/memory/allocator/dyadic.hpp"



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

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(alloc_buffer);
    };

    struct as_copy_t  {};
    const as_copy_t as_copy = {};

    template <
    typename T,
    typename COMPARATOR,
    typename MEM_BUFFER>
    class heap
    {
    public:
        YACK_DECL_ARGS(T,type);

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

        virtual inline size_t size()     const noexcept { return Q.count; }
        virtual inline size_t capacity() const noexcept { return Q.total; }

        inline const prio_queue<T,COMPARATOR> & getQ() const noexcept { return Q; }

    private:
        MEM_BUFFER               M;
        prio_queue<T,COMPARATOR> Q;

        YACK_DISABLE_COPY_AND_ASSIGN(heap);
    };


}

YACK_UTEST(data_heap)
{
    randomized::rand_ ran;


    heap< int,icompare,fixed_buffer<int,5> >              fih;
    heap< int,icompare,alloc_buffer<int,memory::pooled> > dih1;
    heap< int,icompare,alloc_buffer<int,memory::dyadic> > dih2(7);

    fih.push(1);

    std::cerr << "fih:  " << fih.size()  << " / " << fih.capacity()  << std::endl;
    std::cerr << "dih1: " << dih1.size() << " / " << dih1.capacity() << std::endl;
    std::cerr << "dih2: " << dih2.size() << " / " << dih2.capacity() << std::endl;

    {
        heap< int,icompare,fixed_buffer<int,5> >              fih_copy1(fih,as_copy);
        heap< int,icompare,alloc_buffer<int,memory::pooled> > fih_copy2(fih,as_copy);
        heap< int,icompare,alloc_buffer<int,memory::dyadic> > fih_copy3(fih,as_copy);

    }



    return 0;

    {
        { alloc_buffer<int,memory::pooled> demo; }

        alloc_buffer<int,memory::pooled>  ab(12);
        std::cerr << "#items=" << ab.num_items << " / #bytes=" << ab.num_bytes << std::endl;


        {
            fixed_buffer<int,4> data;
            prio_queue<int,icompare> Q(data.workspace,data.num_items);

            Q.insert(-1); std::cerr <<  "-> " <<  Q.tree[0] << std::endl;
            Q.insert(2);  std::cerr <<  "-> " <<  Q.tree[0] << std::endl;
            Q.insert(-3); std::cerr <<  "-> " <<  Q.tree[0] << std::endl;
            Q.insert(4);  std::cerr <<  "-> " <<  Q.tree[0] << std::endl;
        }

        {
            fixed_buffer<string,8> data;
            prio_queue<string,scompare> Q(data.workspace,data.num_items);
            while(Q.count<Q.total)
            {
                const string tmp = bring::get<string>(ran);
                Q.insert(tmp);
                std::cerr << "-> " << tmp << " -> " << Q.tree[0] << std::endl;
            }
        }
    }



}
YACK_UDONE()
