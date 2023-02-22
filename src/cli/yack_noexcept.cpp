#include "yack/noexcept.hpp"
#include "yack/comparison.hpp"
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <ctime>

static inline
void display_version(const int A, const int B, const int C,
                     const int D, const int E, const int F)
{



    std::cerr << "? [" << A << "." << B << "." << C << "]"
    << " <"
    << " [" << D << "." << E << "." << F << "] = ";

    const int res = YACK_VERSION_LESSER(A, B, C, D, E, F);

    const int  lhs[3] = {A,B,C};
    const int  rhs[3] = {D,E,F};
    const bool cmp    = yack::comparison::lexicographic(lhs,rhs,3) < 0;

    std::cerr << res << " / " << cmp << std::endl;
    if(res!=cmp)
    {
        std::cerr << "bad result!" << std::endl;
        exit(1);
    }

}

static inline int getInt()
{
    return rand() % 20;
}

int main(int, char**)
{
    srand( static_cast<unsigned>(time(NULL)) );
    fprintf(stdout,"<%u.%u.%u>\n", YACK_MAJOR, YACK_MINOR, YACK_PATCH);

    display_version(4,8,0,4,8,1);
    display_version(4,8,1,4,8,1);
    for(int i=0;i<1000;++i)
    {
        display_version(getInt(),getInt(),getInt(),getInt(),getInt(),getInt());
    }

    return 0;
}
