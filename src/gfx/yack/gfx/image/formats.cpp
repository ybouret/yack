#include "yack/gfx/image/formats.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace graphic
    {

        namespace image
        {
            const char formats:: call_sign[] = "graphic::images";
            
            formats:: ~formats() noexcept
            {
            }

            formats:: formats() :
            singleton<formats>(),
            codec(call_sign),
            fmt_set(),
            fdb()
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

            void formats:: save(const pixmap<rgba> &surface, const string &filename, const options *opts) const
            {
                format_for(filename).save(surface,filename,opts);
            }


            void formats:: decl(format *addr)
            {
                assert(NULL!=addr);
                const fmt_ptr ptr = addr;
                if(!insert(ptr)) throw exception("%s: multiple '%s'",call_sign,ptr->name());
            }

            

            const format * formats::query(const string &id) const noexcept
            {
                const fmt_ptr *ppC = search(id);
                return NULL!=ppC ? & **ppC : NULL;

            }

            const format * formats::query(const char *id) const noexcept
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

            void formats:: decl(filter *f)
            {
                assert(NULL!=f);
                const filter_ptr ptr = f;
                if(! coerce(fdb).insert(ptr)) throw exception("%s: multiple '%s'", call_sign, ptr->name());
            }

            const filter & formats:: fetch(const string &id) const
            {
                const filter_ptr *ppF = fdb.search(id);
                if(!ppF) throw exception("%s: no filter '%s'",call_sign,id());
                return **ppF;
            }

            const filter & formats:: fetch(const char *id) const
            {
                const string _(id);
                return fetch(_);
            }

            filters formats:: carve(const string &root) const
            {
                const string xid = root + filters::Xext;
                const string yid = root + filters::Yext;
                const filter &fx = fetch(xid);
                const filter &fy = fetch(yid);
                return filters(fx,fy);
            }

            filters formats:: carve(const char *root) const
            {
                const string _(root);
                return carve(_);
            }


        }

    }

}

