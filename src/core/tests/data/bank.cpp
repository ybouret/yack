#include "yack/data/small/set.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(data_bank)
{
    small_bank<int>::pointer sharedBank( new small_bank<int>() );
    small_set<int>           iSet( sharedBank );

    iSet.add(4);
    iSet.pre(5);

    std::cerr << *iSet << std::endl;

    iSet.free();

    YACK_CHECK(iSet.insert(3));
    iSet.include(4);
    iSet.include(4);
    YACK_CHECK(!iSet.insert(3));
    std::cerr << *iSet << std::endl;

    YACK_CHECK(iSet.remove(4));
    YACK_CHECK(!iSet.remove(4));


    iSet += 1;
    iSet += 2;
    iSet << 3 << 4 << 5 << 6 << 7 << 8 << 3;

    std::cerr << *iSet << std::endl;

    iSet -= 3;
    std::cerr << *iSet << std::endl;
    {
        small_set<int> temp( sharedBank );
        temp << 1 << 4 << 7;
        iSet += temp;
        std::cerr << *iSet << std::endl;
        iSet -= temp;
        std::cerr << *iSet << std::endl;
    }

    {
        small_set<int> iSet2(iSet);
        std::cerr << *iSet2 << std::endl;
        iSet2.free();
        iSet2 = iSet;
        std::cerr << *iSet2 << std::endl;
    }




}
YACK_UDONE()

