
#include "yack/system/rtti.hpp"
#include "yack/string.hpp"

namespace yack
{

    namespace kernel
    {
        rtti:: rtti(const yack::string &name) :
        next(0),
        impl( new yack::string(name) )
        {
        }

        rtti:: ~rtti() throw()
        {
            assert(NULL==next);
            assert(NULL!=impl);
            delete impl;
            impl = 0;
        }

        const yack::string & rtti:: operator*() const throw()
        {
            assert(impl);
            return *impl;
        }

        
    }

}

namespace yack
{

    rtti:: ~rtti() throw()
    {
    }




    rtti:: rtti(const std::type_info &ti)
    {
        const string _( ti.name() );
        store( new kernel::rtti(_) );
    }

    const string & rtti:: name() const throw()
    {
        return **head;
    }


    std::ostream & operator<<(std::ostream &os, const rtti &r)
    {
        assert(r.size>0);
        const kernel::rtti *node = r.head;
        os << '[';
        os << '<' << **node << '>';
        while( NULL != (node=node->next) )
        {
            os << " <- <" << **node << ">";
        }
        os << ']';
        return os;
    }

}

#include "yack/singleton.hpp"
#include "yack/associative/suffix/map.hpp"
#include "yack/ptr/arc.hpp"
#include "yack/exception.hpp"

namespace yack
{
    typedef arc_ptr<rtti>               rtti_ptr;
    typedef suffix_map<string,rtti_ptr> rtti_map;

    class rtti_db : public singleton<rtti_db>, public rtti_map
    {
    public:
        static const char                   call_sign[];
        static const at_exit::longevity     life_time = 0;

        rtti_ptr *get(const char *path, const size_t plen)   throw()
        {
            return tree.search(path,plen);
        }

        rtti_ptr *get(const char *path)   throw()
        {
            assert(path);
            return tree.search(path,strlen(path));
        }

        rtti_ptr *get(const string &path)   throw()
        {
            return tree.search(path(),path.size());
        }


        rtti & use(const std::type_info &ti)
        {
            YACK_LOCK(access);

            const char   *id = ti.name();
            rtti_ptr     *pp = get(id);
            if(pp)
            {
                // already present
                return **pp;
            }
            else
            {
                rtti_ptr     p( new rtti(ti) );
                const string path = p->name();
                if(!tree.insert(p,path(),path.size()))
                {
                    throw exception("%s failure for <%s>",call_sign,id);
                }
                return *p;
            }
        }

        rtti & use(const std::type_info &ti, const string &alias)
        {
            YACK_LOCK(access);
            const char  *id = ti.name();
            const string ID = id;
            rtti_ptr    *pp = get(alias);
            if(pp)
            {
                //--------------------------------------------------------------
                // alias exists, check same ti
                //--------------------------------------------------------------
                const rtti_ptr *qq = get(ID);
                if(qq)
                {
                    if(& **pp != & **qq)
                    {
                        const string &other = (*qq)->name();
                        throw exception("%s cannot alias <%s> to <%s>, in use for <%s>", call_sign, id, alias(), other() );
                    }
                }
                return **pp;
            }
            else
            {
                //--------------------------------------------------------------
                // alias doesn't exists : get principal rtti and add alias
                //--------------------------------------------------------------
                rtti &self = use(ti);
                self.store( new kernel::rtti(alias) );

                //--------------------------------------------------------------
                // register global alias
                //--------------------------------------------------------------
                try
                {
                    const rtti_ptr temp = &self;
                    if(!tree.insert(temp,alias(),alias.size()))
                    {
                        throw exception("%s insert alias <%s> for <%s> failure", call_sign, alias(), id);
                    }
                }
                catch(...)
                {
                    delete self.query();
                    throw;
                }
                return self;
            }

        }

        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(rtti_db);
        friend class singleton<rtti_db>;
        inline virtual ~rtti_db() throw() {}
        inline explicit rtti_db() throw() : singleton<rtti_db>(), rtti_map()
        {
            setup();
        }

        void setup();
    };

    const char rtti_db:: call_sign[] = "rtti";

    const rtti & rtti::use(const std::type_info &ti)
    {
        static rtti_db &db = rtti_db::instance();
        return db.use(ti);
    }

    const rtti & rtti:: use(const std::type_info &ti, const string &alias)
    {
        static rtti_db &db = rtti_db::instance();
        return db.use(ti,alias);
    }

    const rtti & rtti:: use(const std::type_info &ti, const char *alias)
    {
        const string  _(alias);
        return use(ti,_);
    }

    void rtti:: gv()
    {
        static const rtti_db &db = rtti_db::instance();
        db.tree.gv("rtti.dot");
        ios::vizible::render("rtti.dot");
    }

    void rtti::display()
    {
        static const rtti_db &db = rtti_db::instance();
        std::cerr << "<rtti count=" << db.size() << ">" << std::endl;
        for(rtti_db::const_iterator it=db.begin();it!=db.end();++it)
        {
            std::cerr << "    " << *it << std::endl;
        }
        std::cerr << "<rtti/>" << std::endl;
    }


}


namespace yack
{
#define YACK_RTTI(TYPE) do { const string alias = #TYPE; (void) use( typeid(TYPE), alias); } while(false)

    void rtti_db:: setup()
    {
        YACK_RTTI(int8_t);
        YACK_RTTI(int16_t);
        YACK_RTTI(int32_t);
        YACK_RTTI(int64_t);
    }
}
