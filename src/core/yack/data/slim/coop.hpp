
//! \file

#ifndef YACK_DATA_SLIM_COOP_INCLUDED
#define YACK_DATA_SLIM_COOP_INCLUDED 1

#include "yack/data/slim/bank.hpp"

namespace yack
{

    template <typename T, typename NODE, template <typename,typename> class CATALOG>
    class slim_coop : public CATALOG<T, typename slim_bank<NODE>::pointer >
    {
    public:
        typedef NODE                 node_type;
        typedef slim_bank<node_type> bank_type;
        typedef typename bank_type::pointer fund_type;
        typedef CATALOG<T,fund_type> catalog_t;

        inline explicit slim_coop(const fund_type &fund) throw() : catalog_t(fund) {}
        inline virtual ~slim_coop() throw() {}
        inline          slim_coop(const slim_coop &other) : catalog_t(other) {}

    private:
        YACK_DISABLE_ASSIGN(slim_coop);
    };

}

#endif

