
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

        std::ostream & operator<<(std::ostream & os, const mutex &m) noexcept
        {
            try
            {
                os << "[mutex@" << (const void*)(m.impl) << " <" << m.name << ">]";
            }
            catch(...)
            {
            }
            return os;
        }

        void mutex::tag(const char *id) noexcept
        {
            static const size_t max_len = sizeof(name)-1;
            const        size_t tag_len = id ? strlen(id) : 0;
            const        size_t cpy_len = min_of(max_len,tag_len);

            for(size_t i=0;i<cpy_len;++i)       coerce(name[i]) = id[i];
            for(size_t i=cpy_len;i<max_len;++i) coerce(name[i]) = 0;

            if(verbose) std::cerr << "+" << *this << std::endl;

        }


        mutex:: ~mutex() noexcept
        {
            assert(impl);
            if(verbose) std::cerr << "-" << *this << std::endl;
            quark::mutex_api::quit(impl);
            assert(!impl);
        }

        void mutex:: lock() noexcept
        {
            assert(impl);
            quark::mutex_api::lock(impl);
            if(verbose) std::cerr << ">" << *this << " [LOCKED]" << std::endl;
        }


        void mutex:: unlock() noexcept
        {
            assert(impl);
            if(verbose) std::cerr << "<" << *this << " [UNLOCKING]" << std::endl;
            quark::mutex_api::unlock(impl);
        }

        bool mutex:: try_lock() noexcept
        {
            assert(impl);
            return quark::mutex_api::try_lock(impl);
        }
    }

}

