#include "yack/gfx/image/formats.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace graphic
    {

        namespace image
        {
            const char formats:: call_sign[] = "graphic::images";
            
            formats:: ~formats() throw()
            {
            }

            formats:: formats() :
            singleton<formats>(),
            codec(call_sign),
            fmt_set()
            {
            }


            const format & formats:: format_for(const string &filename) const
            {
                YACK_LOCK(access);
                for(const knot_type *node = (*get_tree()).head;node;node=node->next)
                {
                    format &io =  coerce(***node);
                    if(io.matches(filename)) return io;
                }
                throw exception("%s: no codec for '%s'", call_sign, filename() );
            }

            pixmap<rgba> formats:: load(const string &filename, const options *opts) const
            {
                return format_for(filename).load(filename,opts);
            }

            void formats:: decl(format *addr)
            {
                assert(NULL!=addr);
                const fmt_ptr ptr = addr;
                if(!insert(ptr)) throw exception("%s: multiple '%s'",call_sign,ptr->name());
            }

            const format * formats::query(const string &id) const throw()
            {
                const fmt_ptr *ppC = search(id);
                return NULL!=ppC ? & **ppC : NULL;

            }

            const format * formats::query(const char *id) const throw()
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

