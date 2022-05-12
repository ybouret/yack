#include "yack/gfx/image/codecs.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace graphic
    {

        namespace image
        {
            const char codecs:: call_sign[] = "graphic::images";
            
            codecs:: ~codecs() throw()
            {
            }

            codecs:: codecs() :
            singleton<codecs>(),
            codec(call_sign),
            codecs_db()
            {
            }

            pixmap<rgba> codecs:: load(const string &, const options *) const
            {
                throw exception("not implemented");
            }

            void codecs:: decl(codec *addr)
            {
                assert(NULL!=addr);
                const codec_ptr ptr = addr;
                if(!insert(ptr)) throw exception("%s: multiple '%s'",call_sign,ptr->name());
            }

            const codec * codecs::query(const string &id) const throw()
            {
                const codec_ptr *ppC = search(id);
                return NULL!=ppC ? & **ppC : NULL;

            }

            const codec * codecs::query(const char *id) const throw()
            {
                if(id)
                {
                    const codec_ptr *ppC = get_tree().search(id,strlen(id));
                    return NULL!=ppC ? & **ppC : NULL;
                }
                else
                {
                    return NULL;
                }
            }


        }

    }

}

