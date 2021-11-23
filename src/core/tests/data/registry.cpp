#include "yack/associative/hash/set.hpp"
#include "yack/associative/suffix/set.hpp"

#include "yack/sequence/vector.hpp"
#include "../main.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

class dummy : public object
{
public:
    const string k;
    const size_t u;
    
    dummy(const string &key, const size_t value) :
    object(),
    k(key),
    u(value)
    {
    }
    
    dummy(const dummy &d) : object(),
    k(d.k),
    u(d.u)
    {
    }
    
    
    ~dummy() throw()
    {
    }
    
    friend std::ostream & operator<<( std::ostream &os, const dummy &d)
    {
        os << '[' << d.k << '@' << d.u << ']';
        return os;
    }
    
    const string & key() const throw() { return k; }
    
private:
    YACK_DISABLE_ASSIGN(dummy);
};


YACK_UTEST(data_registry)
{
    randomized::rand_ ran;
    {
        size_t            n = 10;
        vector<string>    keys(n,as_capacity);
        
        for(size_t i=n;i>0;--i)
        {
        TRIAL:
            const string tmp = bring::get<string>(ran);
            for(size_t j=keys.size();j>0;--j)
            {
                if(tmp==keys[j]) goto TRIAL;
            }
            keys.push_back(tmp);
        }
        
        std::cerr << "keys=" << keys << std::endl;
        
        suffix_set<string,dummy> s_set;
        hash_set<string,dummy>   h_set;
        for(size_t i=1;i<=n;++i)
        {
            const dummy d(keys[i],i);
            YACK_ASSERT(s_set.insert(d));
            YACK_ASSERT(h_set.insert(d));
        }
        
        randomized::shuffle::data( keys(), keys.size(), ran);
        std::cerr << "keys=" << keys << std::endl;
        for(size_t i=1;i<=n;++i)
        {
            YACK_ASSERT(s_set.search(keys[i]));
            YACK_ASSERT(h_set.search(keys[i]));
        }
        
        std::cerr << s_set << std::endl;
        std::cerr << h_set << std::endl;

    }
    
}
YACK_UDONE()

