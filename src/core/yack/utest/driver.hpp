//! \file

#ifndef YACK_UTEST_DRIVER_INCLUDED
#define YACK_UTEST_DRIVER_INCLUDED 1

#include "yack/setup.hpp"
#include <cstring>
#include <iostream>
#include <iomanip>

namespace Yack
{
    struct UTest
    {
        typedef int (*Proc)(int argc, const char **argv);
        static  int ReturnTooManyTests(const size_t maxCount);
        static  int ReturnAlreadyTests(const char  *name);
        static  int Compare(const void *, const void *) throw();
        void        display(std::ostream &, const size_t width) const;
        bool        isAlike(const char *other) const throw();

        const char *  name;
        Proc          proc;


    };

    template <size_t N>
    class UTests
    {
    public:
        inline UTests() throw() : count(0), width(0), utest()
        {
            clear();
        }

        inline ~UTests() throw()
        {
            clear();
            count=0;
            width=0;
        }

        int operator()(const char * const name,
                        UTest::Proc        proc) throw()
        {
            assert(NULL!=name);
            assert(NULL!=proc);
            if(count>=N)     UTest::ReturnTooManyTests(N);
            if(search(name)) UTest::ReturnAlreadyTests(name);
            UTest &t = utest[count++];
            t.name = name;
            t.proc = proc;
            const size_t w     = strlen(name);
            if(w>width)  width = w;
            return 0;
        }

        inline UTest *search(const char *name) throw()
        {
            assert(NULL!=name);
            for(size_t i=0;i<count;++i)
            {
                UTest *t = &utest[i];
                if( 0 == strcmp(t->name,name) ) return t;
            }
            return 0;
        }

        int operator()(int            argc,
                       const char **  argv)
        {
            qsort(utest,count,sizeof(UTest),UTest::Compare);
            if(argc<=1)
            {
                std::cout << "#" << argv[0] << " = " << count << std::endl;
                for(size_t i=0;i<count;++i)
                {
                    utest[i].display(std::cout << '\t',width);
                }
                return 0;
            }
            else
            {
                const char *name = argv[1]; assert(name);
                UTest      *test = search(name);
                if(test)
                {
                    return test->proc(--argc,++argv);
                }
                else
                {
                    std::cerr << "No [" << name << "] in " << argv[0] << std::endl;
                    bool first = true;
                    for(size_t i=0;i<count;++i)
                    {
                        const UTest &test = utest[i];
                        if(test.isAlike(name))
                        {
                            if(first)
                            {
                                std::cerr << "Alike:" << std::endl;
                                first = false;
                            }
                            test.display(std::cerr << '\t',width);
                        }
                    }
                    return 1;
                }
            }
        }


    private:
        size_t count;
        size_t width;
        UTest  utest[N];
        YACK_DISABLE_COPY_AND_ASSIGN(UTests);
        void clear() throw() { memset(utest,0,sizeof(utest)); }
    };


}

#define YACK_UTEST_DECL(COUNT)         \
/**/  int main(int argc, const char **argv) { \
/**/   Yack::UTests<COUNT> utests;

#define YACK_UTEST(NAME) do { \
/**/ extern Yack::UTest::Proc YACK_Test_##NAME;\
/**/ static const char name[] = #NAME;\
/**/ const  int        ret    = utests(name,YACK_Test_##NAME);\
/**/ } while(false)

#define YACK_UTEST_EXEC()         \
/**/    return utests(argc,argv); \
/**/  }


#endif


