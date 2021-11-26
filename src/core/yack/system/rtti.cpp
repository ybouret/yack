
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

    const string & rtti:: native() const throw()
    {
        return **tail();
    }



    std::ostream & operator<<(std::ostream &os, const rtti &r)
    {
        assert(r.size>0);
        const kernel::rtti *node = r.head;
        os << '<' << **node;
        while( NULL != (node=node->next) )
        {
            os << '|' << **node;
        }
        os << '>';
        return os;
    }

}

#include "yack/singleton.hpp"
#include "yack/associative/suffix/map.hpp"
#include "yack/ptr/arc.hpp"
#include "yack/exception.hpp"
#include "yack/memory/allocator/pooled-longevity.hpp"

namespace yack
{
    typedef arc_ptr<rtti>               rtti_ptr;
    typedef suffix_map<string,rtti_ptr> rtti_map;

    class rtti_db : public singleton<rtti_db>, public rtti_map
    {
    public:
        static const char                   call_sign[];
        static const at_exit::longevity     life_time = YACK_MEMORY_POOLED_ALLOCATOR_LONGEVITY-1;

        rtti_ptr *get(const char   *path, const size_t plen) throw() { return tree.search(path,plen);          }
        rtti_ptr *get(const char   *path) throw() { assert(path);      return tree.search(path,strlen(path));  }
        rtti_ptr *get(const string &path) throw() {                    return tree.search(path(),path.size()); }


        rtti & use(const std::type_info &ti)
        {
            YACK_LOCK(access);
            const char   *id = ti.name();
            rtti_ptr     *pp = get(id);
            if(pp)
            {
                //--------------------------------------------------------------
                // already present : recall
                //--------------------------------------------------------------
                return **pp;
            }
            else
            {
                //--------------------------------------------------------------
                // not present: create
                //--------------------------------------------------------------
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
            rtti           &self = use(ti);
            const rtti_ptr *pp   = get(alias);
            if(pp)
            {
                // alias already exist
                const rtti &info = **pp;
                if(&self != &info)
                {
                    const string &me = self.name();
                    const string &it = info.name();
                    throw exception("%s cannot alias <%s> to <%s>, in use for <%s>", call_sign, me(), alias(), it() );
                }
            }
            else
            {
                //--------------------------------------------------------------
                // append alias
                //--------------------------------------------------------------
                self.store( new kernel::rtti(alias) );

                //--------------------------------------------------------------
                // register global alias
                //--------------------------------------------------------------
                try
                {
                    const rtti_ptr temp = &self;
                    if(!tree.insert(temp,alias(),alias.size()))
                    {
                        const string &me = self.name();
                        throw exception("%s insert alias <%s> for <%s> failure", call_sign, alias(), me());
                    }
                }
                catch(...)
                {
                    delete self.query();
                    throw;
                }
            }


            return self;
            
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

    const string & rtti::name(const std::type_info &ti)
    {
        static rtti_db &db = rtti_db::instance();
        return db.use(ti).name();
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

        YACK_RTTI(char);
        YACK_RTTI(short);
        YACK_RTTI(int);
        YACK_RTTI(long);
        YACK_RTTI(long long);


        YACK_RTTI(unit_t);



        YACK_RTTI(uint8_t);
        YACK_RTTI(uint16_t);
        YACK_RTTI(uint32_t);
        YACK_RTTI(uint64_t);

        YACK_RTTI(unsigned char);
        YACK_RTTI(unsigned short);
        YACK_RTTI(unsigned);
        YACK_RTTI(unsigned long);
        YACK_RTTI(unsigned long long);

        YACK_RTTI(size_t);

        YACK_RTTI(float);
        YACK_RTTI(double);
        YACK_RTTI(long double);

        YACK_RTTI(bool);


    }
}
