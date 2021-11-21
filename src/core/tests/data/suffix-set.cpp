
#include "yack/associative/suffix/set.hpp"

#include "yack/utest/run.hpp"

using namespace yack;

namespace
{
    class dummy : public object
    {
    public:
        const int k;
        
        dummy(const int a) throw() : object(), k(a)
        {
        }
        
        dummy(const dummy &d) throw() : object(), k(d.k)
        {
        }
        
        
        ~dummy() throw()
        {
        }
        
        friend std::ostream & operator<<( std::ostream &os, const dummy &d)
        {
            os << '[' << d.k << ']';
            return os;
        }
        
        const int & key() const throw() { return k; }
        
    private:
        YACK_DISABLE_ASSIGN(dummy);
    };
}

YACK_UTEST(data_suffix_set)
{
    randomized::rand_ ran;
    
    {
        suffix_set<int,dummy> s;
        vector<int> keys;
        for(int i=0;i<10;++i)
        {
            keys.push_back(i);
            const dummy  d(i);
            YACK_ASSERT(s.insert(d));
        }
        std::cerr << "#keys=" << keys.size()     << std::endl;
        std::cerr << "#data=" << s.size()        << std::endl;
        
        for(suffix_set<int,dummy>::const_iterator it=s.begin();it!=s.end();++it)
        {
            const suffix_set<int,dummy>::knot_type *kn = it.operator->();
            kn->node;
            //const int k = kn->k;
            const dummy *d = (*kn).operator->();
            //const int    k = d->k;
            //const int    kk = it->k;
            // std::cerr << *it << ", key=" << it->key() << std::endl;
            std::cerr << *it << std::endl;
            
        }
        
        {
            //hash_set<int,dummy> tmp(s);
            //YACK_CHECK(tmp.size()==s.size());
        }
        
        randomized::shuffle::data( keys(), keys.size(), ran);
        
        while(keys.size())
        {
            const int &key = keys.back();
            YACK_ASSERT(  s.search(key)  );
            YACK_ASSERT(  s.remove(key)  );
            YACK_ASSERT( !s.search(key) );
            YACK_ASSERT( !s.remove(key) );
            keys.pop_back();
        }
        std::cerr << "#keys=" << keys.size() << std::endl;
        std::cerr << "#data=" << s.size() << std::endl;
     }
    
}
YACK_UDONE()

