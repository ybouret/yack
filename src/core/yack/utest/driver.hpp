//! \file

#ifndef YACK_UTEST_DRIVER_INCLUDED
#define YACK_UTEST_DRIVER_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/check/static.hpp"

#include <cstring>
#include <iostream>
#include <iomanip>

namespace yack
{

    class unit_test
    {
    public:
        typedef int (*proc)(int,const char **);

        unit_test(const char *name, proc func) throw();
        ~unit_test() throw();

        void display(std::ostream &, const size_t width) const;
        bool is_near(const char *other) const throw();

        const char *  const name;
        proc                func;

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(unit_test);
    };

    class unit_tests
    {
    public:
        virtual ~unit_tests() throw();

        int  operator()(const char     *name,
                        unit_test::proc func) throw();

        int operator()(int            argc,
                       const char **  argv) throw();

    protected:
        explicit unit_tests(void *addr, const size_t size) throw();

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(unit_tests);
        void       clear() throw();
        unit_test *query(const char *name) throw();

        unit_test   *utest;     //!< repository
        size_t       count;     //!< declared
        size_t       width;     //!< max name length
        const size_t capacity;  //!< initial capacity
    };


    template <size_t N>
    class unit_tests_provider : public unit_tests
    {
    public:
        inline virtual ~unit_tests_provider() throw() {}
        inline explicit unit_tests_provider() throw() :
        unit_tests(mock,N), mock() {
            YACK_STATIC_CHECK(sizeof(mock_t)==sizeof(unit_test),MismatchSizeofTest);
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(unit_tests_provider);
        struct mock_t { void *head; void *tail; };
        mock_t mock[N];
    };

}

#define YACK_UTEST_DECL(COUNT)                \
/**/  int main(int argc, const char **argv) { \
/**/  static yack::unit_tests_provider<COUNT> utests;

#define YACK_UTEST(NAME) do {                                       \
/**/ int yack_test_##NAME(int,const char**);                        \
/**/ static const char       name[] = #NAME;                        \
/**/ const        int        ret    = utests(name,yack_test_##NAME);\
/**/ if(0!=ret) return ret;                                         \
/**/ } while(false)

#define YACK_UTEST_EXEC()         \
/**/    return utests(argc,argv); \
/**/  }


#endif


