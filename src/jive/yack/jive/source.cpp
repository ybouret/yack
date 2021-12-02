

#include "yack/jive/source.hpp"

namespace yack
{
    namespace jive
    {
        source:: ~source() throw()
        {
        }

        source:: source(module *m) throw() :
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

        void source::store(character *ch) throw()
        {
            assert(ch);
            cache.push_front(ch);
        }

        void source:: store(token &t) throw()
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


    }
}


