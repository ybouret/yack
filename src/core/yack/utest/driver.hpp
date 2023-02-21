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

    //__________________________________________________________________________
    //
    //
    //! a unit test record
    //
    //__________________________________________________________________________
    class unit_test
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef int (*proc)(int, char **); //!< local program interface

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        unit_test(const char *, proc) noexcept; //!< setup
        ~unit_test()                  noexcept; //!< cleanup

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        void display(std::ostream &, const size_t) const; //!< display centered
        bool is_near(const char *other) const noexcept;    //!< fuzzy search

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const char *  const name; //!< test name
        proc                func; //!< test entry point

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(unit_test);
    };

    //__________________________________________________________________________
    //
    //
    //! a collection of unit tests
    //
    //__________________________________________________________________________
    class unit_tests
    {
    public:
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! record a new test, check if enough space and unique name
        int  operator()(const char     *name,
                        unit_test::proc func) noexcept;

        //! display tests or try to launch one with remaining args
        int operator()(int      argc,
                       char **  argv) noexcept;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        virtual ~unit_tests() noexcept;

    protected:
        //! setup with user's memory
        explicit unit_tests(void *addr, const size_t size) noexcept;

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(unit_tests);
        void       clear() noexcept;
        unit_test *query(const char *name) noexcept;

        unit_test   *utest;     //!< repository
        size_t       count;     //!< declared
        size_t       width;     //!< max name length
        const size_t capacity;  //!< initial capacity
    };


    //__________________________________________________________________________
    //
    //
    //! provider of memory for a given number of unit tests
    //
    //__________________________________________________________________________
    template <size_t N>
    class unit_tests_provider : public unit_tests
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~unit_tests_provider() noexcept {}

        //! setup with internal memory
        inline explicit unit_tests_provider() noexcept :
        unit_tests(mock,N), mock() {
            YACK_STATIC_CHECK(sizeof(mock_t)==sizeof(unit_test),MismatchSizeofTest);
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(unit_tests_provider);
        struct mock_t { void *head; void *tail; };
        mock_t mock[N];
    };

}

//! declare a COUNT of possible unit tests
#define YACK_UTEST_DECL(COUNT)                \
/**/  int main(int argc, char **argv) { \
/**/  static yack::unit_tests_provider<COUNT> utests;

//! register a new test
#define YACK_UTEST(NAME) do {                                       \
/**/ int yack_test_##NAME(int,char**);                        \
/**/ static const char       name[] = #NAME;                        \
/**/ const        int        ret    = utests(name,yack_test_##NAME);\
/**/ if(0!=ret) return ret;                                         \
/**/ } while(false)

//! run/display tests
#define YACK_UTEST_EXEC()         \
/**/    return utests(argc,argv); \
/**/  }


#endif


