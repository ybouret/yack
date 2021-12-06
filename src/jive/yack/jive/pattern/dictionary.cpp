
#include "yack/jive/pattern/dictionary.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {
        dictionary:: ~dictionary() throw()
        {
        }

        dictionary:: dictionary() throw() : dictionary_()
        {
        }

        static const char fn[] = "dictionary: ";

        const pattern * dictionary:: query(const string &key) const throw()
        {
            const motif *ptr = search(key);
            if(ptr) return & **ptr; else return NULL;
        }

        static inline size_t check_dict_key(const char *key)
        {
            if(!key) throw exception("%sNULL key",fn);
            const size_t len = strlen(key); if(len<=0) throw exception("%sempty key",fn);
            return len;
        }

        const pattern * dictionary:: query(const char *key) const
        {
            const size_t len = check_dict_key(key);
            const motif *ptr = tree.search(key,len);
            if(ptr) return & **ptr; else return NULL;
        }

        void dictionary:: operator()(const string &key, pattern *p)
        {
            assert(NULL!=p);
            const motif m(p);
            if(!insert(key,p)) throw exception("%smutliple '%s'",fn,key());
        }

        void dictionary:: operator()(const char *key, pattern *p)
        {
            assert(NULL!=p);
            const motif  m(p);
            const size_t len = check_dict_key(key);
            if(!tree.insert(m,key,len)) throw exception("%smultiple '%s'",fn,key);
        }


    }

}
