

#include "yack/jive/source.hpp"

namespace yack
{
    namespace jive
    {
        source:: ~source() noexcept
        {
        }

        source:: source(module *m) noexcept :
        intake(m),
        cache()
        {
            assert(intake.is_valid());
        }

        character *source:: query()
        {
            if(cache.size)
            {
                return cache.pop_front();
            }
            else
            {
                return intake->get();
            }
        }

        void source::store(character *ch) noexcept
        {
            assert(ch);
            cache.push_front(ch);
        }

        void source:: store(token &t) noexcept
        {
            cache.merge_front(t);
        }

        void source:: store_copy(const token &t)
        {
            cache.merge_front_copy(t);
        }

        const character *source:: peek()
        {
            if(cache.size)
            {
                return cache.head;
            }
            else
            {
                character *ch = intake->get();
                if(ch)
                {
                    return cache.push_front(ch);
                }
                else
                {
                    return NULL; // really no more data
                }
            }
        }

        bool source:: done()
        {
            return NULL == peek();
        }

        void source:: skip() noexcept
        {
            assert(cache.size>0);
            delete cache.pop_front();
        }

        void source:: skip(size_t n) noexcept
        {
            assert(cache.size>=n);
            while(n-- > 0)
            {
                assert(cache.size>0);
                delete cache.pop_front();
            }
        }

        module & source:: operator*() noexcept
        {
            assert(intake.is_valid());
            return *intake;
        }

        size_t source:: read() const noexcept
        {
            return cache.size;
        }
    }
}


