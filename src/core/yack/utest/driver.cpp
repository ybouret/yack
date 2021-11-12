#include "yack/utest/driver.hpp"
#include "yack/type/out-of-reach.hpp"

namespace yack
{
    unit_test:: ~unit_test() throw()
    {

    }

    unit_test:: unit_test(const char *name_, proc func_) throw() :
    name(name_),
    func(func_)
    {
        assert(NULL!=name);
        assert(NULL!=func);
    }

    void unit_test:: display(std::ostream &os, const size_t width) const
    {
        const size_t w = strlen(name);assert(w<=width);
        const size_t d = width-w;
        const size_t prolog = d/2;
        const size_t epilog = d-prolog;
        os << '[';
        for(size_t i=0;i<prolog;++i) os << ' ';
        os << name;
        for(size_t i=0;i<epilog;++i) os << ' ';
        os << ']' << std::endl;
    }


    bool unit_test:: is_near(const char *other) const throw()
    {
        assert(NULL!=other);

        const char *big    = name;
        const char *little = other;

        if(strlen(little)>strlen(big))
        {
            cswap(big,little);
        }
        
        return NULL!=strstr(big,little);
    }


    void unit_tests:: clear() throw()
    {
        out_of_reach::zset(utest,sizeof(unit_test)*capacity);
    }

    unit_tests:: ~unit_tests() throw()
    {
        clear();
    }

    unit_tests:: unit_tests(void *addr, const size_t size) throw() :
    utest( static_cast<unit_test*>(addr) ),
    count(0),
    width(0),
    capacity(size)
    {
        assert(NULL!=addr);
        clear();
    }

    unit_test *unit_tests:: query(const char *name) throw()
    {
        assert(NULL!=name);
        for(size_t i=0;i<count;++i)
        {
            unit_test *t = &utest[i]; assert(NULL!=t->name);
            if( 0 == strcmp(t->name,name) )
            {
                return t;
            }
        }
        return NULL;
    }


    int  unit_tests:: operator()(const char     *name,
                                 unit_test::proc func) throw()
    {
        assert(NULL!=name);
        assert(NULL!=func);

        if(count>=capacity) {
            std::cerr << "*** Exceeded capacity of #" << capacity << " unit tests!" << std::endl;
            return 1;
        }

        if(query(name)) {
            std::cerr << "*** Multiple test name [" << name << "]" << std::endl;
            return 1;
        }

        const size_t w = strlen(name);
        new( &utest[count++] ) unit_test(name,func);
        if(w>width) width = w;
        return 0;
    }

    static inline int compare_unit_tests(const void *lhs, const void *rhs) throw()
    {
        assert(NULL!=lhs);
        assert(NULL!=rhs);
        return strcmp(static_cast<const unit_test *>(lhs)->name,
                      static_cast<const unit_test *>(rhs)->name);

    }

    int unit_tests:: operator()(int            argc,
                                const char **  argv) throw()
    {

        qsort(utest,count,sizeof(unit_test),compare_unit_tests);
        const char *program = argv[0];
        if(argc<=1)
        {
            std::cout << "#" << program << " = " << count << std::endl;
            for(size_t i=0;i<count;++i)
            {
                utest[i].display(std::cout << '\t',width);
            }
            std::cout << "#" << program << " = " << count << std::endl;
            return 0;
        }
        else
        {
            const char *name = argv[1]; assert(name);
            unit_test  *test = query(name);
            if(test)
            {
                return test->func(--argc,++argv);
            }
            else
            {
                std::cerr << "No [" << name << "] in " << argv[0] << std::endl;
                bool first = true;
                for(size_t i=0;i<count;++i)
                {
                    const unit_test &test = utest[i];
                    if(test.is_near(name))
                    {
                        if(first)
                        {
                            std::cerr << "near match:" << std::endl;
                            first = false;
                        }
                        test.display(std::cerr << '\t',width);
                    }
                }
                return 1;
            }
        }
    }




}
