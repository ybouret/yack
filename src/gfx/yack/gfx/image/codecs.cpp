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
            formats()
            {
            }

            const format & codecs:: format_for(const string &filename) const
            {
                YACK_LOCK(access);
                for(const node_type *node = (*get_tree()).head;node;node=node->next)
                {
                    format &io =  coerce(***node);
                    if(io.matches(filename)) return io;
                }
                throw exception("%s: no codec for '%s'", call_sign, filename() );
            }

            pixmap<rgba> codecs:: load(const string &filename, const options *opts) const
            {
                return format_for(filename).load(filename,opts);
            }

            void codecs:: decl(format *addr)
            {
                assert(NULL!=addr);
                const fmt_ptr ptr = addr;
                if(!insert(ptr)) throw exception("%s: multiple '%s'",call_sign,ptr->name());
            }

            const format * codecs::query(const string &id) const throw()
            {
                const fmt_ptr *ppC = search(id);
                return NULL!=ppC ? & **ppC : NULL;

            }

            const format * codecs::query(const char *id) const throw()
            {
                if(id)
                {
                    const fmt_ptr *ppC = get_tree().search(id,strlen(id));
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

