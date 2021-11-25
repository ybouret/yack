#include "yack/associative/hash/set.hpp"
#include "yack/associative/suffix/set.hpp"
#include "yack/ptr/ark.hpp"

#include "yack/sequence/vector.hpp"
#include "../main.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

class dummy : public object, public counted
{
public:
    typedef ark_ptr<string,dummy> ptr;
    const string k;
    const size_t u;
    
    dummy(const string &key, const size_t value) :
    object(),
    k(key),
    u(value)
    {
    }
    
    dummy(const dummy &d) : object(), counted(),
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
        
        suffix_set<string,dummy>      s_set;
        hash_set<string,dummy>        h_set;
        suffix_set<string,dummy::ptr> S_set;
        hash_set<string,dummy::ptr>   H_set;

        std::cerr << "creating registries" << std::endl;
        for(size_t i=1;i<=n;++i)
        {
            {
                const dummy d(keys[i],i);
                YACK_ASSERT(s_set.insert(d));
                YACK_ASSERT(h_set.insert(d));
            }

            {
                const dummy::ptr d = new dummy(keys[i],i);
                YACK_ASSERT(S_set.insert(d));
                YACK_ASSERT(H_set.insert(d));
            }
        }

        std::cerr << "make local copies" << std::endl;
        {
            suffix_set<string,dummy>      s_set_copy(s_set); YACK_CHECK(s_set_copy.size()==s_set.size());
            hash_set<string,dummy>        h_set_copy(h_set); YACK_CHECK(h_set_copy.size()==h_set.size());
            suffix_set<string,dummy::ptr> S_set_copy(S_set); YACK_CHECK(S_set_copy.size()==S_set.size());
            hash_set<string,dummy::ptr>   H_set_copy(H_set); YACK_CHECK(H_set_copy.size()==H_set.size());
        }

        randomized::shuffle::data( keys(), keys.size(), ran);
        std::cerr << "keys=" << keys << std::endl;
        for(size_t i=1;i<=n;++i)
        {
            YACK_ASSERT(s_set.search(keys[i]));
            YACK_ASSERT(h_set.search(keys[i]));
            YACK_ASSERT(S_set.search(keys[i]));
            YACK_ASSERT(H_set.search(keys[i]));
        }
        
        std::cerr << "suffix_set = " << s_set << std::endl;
        std::cerr << "hash_set   = " << h_set << std::endl;

    }


}
YACK_UDONE()

