
#include "yack/concurrent/mutex.hpp"
#include "yack/concurrent/quark/mutex.hpp"
#include "yack/type/utils.hpp"
#include <iostream>
#include <cstring>

namespace yack
{
    namespace concurrent
    {

        bool mutex::verbose = false;

        mutex:: mutex(const char *id) :
        primitive(),
        lockable(),
        impl( quark::mutex_api::init() ),
        name()
        {
            if(!id) id=yack_unknown;
            tag(id);
        }

        void mutex::tag(const char *id) throw()
        {
            static const size_t max_len = sizeof(name)-1;
            const        size_t tag_len = id ? strlen(id) : 0;
            const        size_t cpy_len = min_of(max_len,tag_len);

            for(size_t i=0;i<cpy_len;++i)       coerce(name[i]) = id[i];
            for(size_t i=cpy_len;i<max_len;++i) coerce(name[i]) = 0;

            if(verbose)
            {
                std::cerr << "\t+mutex@" << impl << ":'" << name << "'" << std::endl;
            }
        }


        mutex:: ~mutex() throw()
        {
            assert(impl);
            if(verbose) { std::cerr << "\t-mutex@" << impl << ": '" << name << "'" << std::endl; }
            quark::mutex_api::quit(impl);
            assert(!impl);
        }

        void mutex:: lock() throw()
        {
            assert(impl);
            quark::mutex_api::lock(impl);
        }


        void mutex:: unlock() throw()
        {
            assert(impl);
            quark::mutex_api::unlock(impl);
        }

        bool mutex:: try_lock() throw()
        {
            assert(impl);
            return quark::mutex_api::try_lock(impl);
        }
    }

}

