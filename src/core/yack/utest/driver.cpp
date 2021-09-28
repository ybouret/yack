
#include "yack/utest/driver.hpp"

namespace Yack
{
    int UTest:: ReturnTooManyTests(const size_t maxCount)
    {
        std::cerr << "Already declared " << maxCount << " tests!" << std::endl;
        return 1; 
    }

    int UTest:: ReturnAlreadyTests(const char  *name)
    {
        assert(NULL!=name);
        std::cerr << "Already declared [" << name << "]" << std::endl;
        return 1;
    }

    int UTestCompare(const void *lhs, const void *rhs) throw()
    {
        assert(NULL!=lhs);
        assert(NULL!=rhs);
        return strcmp(static_cast<const UTest *>(lhs)->name,
                      static_cast<const UTest *>(rhs)->name);

    }
    
    void UTest:: SortByNames(UTest *utest,const size_t count) throw()
    {
        assert(NULL!=utest);
        qsort(utest,count,sizeof(UTest),UTestCompare);
    }
    
    UTest *UTest:: Search(const char *name, UTest *utest, const size_t count) throw()
    {
        assert(NULL!=name);
        assert(NULL!=utest);
        for(size_t i=0;i<count;++i)
        {
            UTest *t = &utest[i];
            if( 0 == strcmp(t->name,name) ) return t;
        }
        return NULL;
    }
    
    void UTest:: display(std::ostream &os, const size_t width) const
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

    bool UTest:: isAlike(const char *other) const throw()
    {
        assert(NULL!=other);

        const char *big    = name;
        const char *little = other;

        if(strlen(little)>strlen(big))
        {
            Swap(big,little);
        }

        //std::cerr << "comparing " << big << " / " << little << std::endl;

        return NULL!=strstr(big,little);
    }


}
