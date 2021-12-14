
#include "yack/jive/pattern/posix.hpp"
#include "yack/jive/pattern/all.hpp"

namespace yack
{
    namespace jive
    {
        pattern * posix:: lower()
        {
            return new within('a','z');
        }

        pattern * posix:: upper()
        {
            return new within('A','Z');
        }

        pattern * posix:: alpha()
        {
            auto_ptr<op_or> p = new op_or();
            p->push_back( upper() );
            p->push_back( lower() );
            return   p.yield();
        }

        pattern * posix:: digit()
        {
            return new within('0','9');
        }

        pattern * posix:: alnum()
        {
            auto_ptr<op_or> p = new op_or();
            p->push_back( digit() );
            p->push_back( upper() );
            p->push_back( lower() );
            return p.yield();
        }

        pattern * posix:: word()
        {
            auto_ptr<op_or> p = new op_or();
            p->add(  '_'  );
            p->push_back( digit() );
            p->push_back( upper() );
            p->push_back( lower() );
            return p.yield();
        }

        pattern * posix:: xdigit()
        {
            auto_ptr<op_or> p = new op_or();
            p->push_back( digit() );
            p->add('A','F');
            p->add('a','f');
            return   p.yield();
        }

        pattern * posix:: blank()
        {
            static const char data[] = " \t";
            return logical::among(data,sizeof(data)-1);
        }

        pattern * posix:: space()
        {
            return logical::among(" \t\n\r\v\f");
        }

        pattern * posix::punct()
        {
            return logical::among("][!\"#$%&'()*+,./:;<=>?@\\^_`{|}~-");
        }

        static inline void __fill_endl( patterns &ops )
        {
            ops.push_back( new single('\n') );
            ops.push_back( new single('\r') );
            ops.push_back( logical::equal("\r\n") );
        }

        pattern * posix:: endl()
        {
            auto_ptr<op_or> p = new op_or();
            __fill_endl(*p);
            return  pattern::optimize(p.yield());
        }

        pattern * posix:: dot()
        {
            auto_ptr<op_none> p = new op_none();
            __fill_endl(*p);
            return  pattern::optimize(p.yield());
        }

        pattern * posix:: core()
        {
            auto_ptr<op_or> p = new op_or();
            p->add( 0x20 );
            p->add( 0x21 );
            p->add( 0x23,0x26 );
            p->add( 0x28,0x5B );
            p->add( 0x5D,0x7F );
            return pattern::optimize( p.yield() );
        }

        static const char __vowels__[] = "aeiouyAEIOUY";

        pattern * posix:: vowel()
        {
            return logical::among(__vowels__);
        }

        pattern * posix:: consonant()
        {
            return logical::avoid(__vowels__);
        }

    }

}

#include "yack/jive/tags.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {

#define YPOSIX(NAME) \
/**/  do { \
/**/    const char name[] = #NAME;\
/**/    if( ! coerce(pdb).tree.insert(posix::NAME,name,sizeof(name)-1)) throw exception("unable to use %s",name);\
/**/  } while(false)

        void tags:: setup_pdb()
        {
            
                YPOSIX(lower);
                YPOSIX(upper);
                YPOSIX(alpha);
                YPOSIX(digit);
                YPOSIX(alnum);
                YPOSIX(xdigit);
                YPOSIX(blank);
                YPOSIX(space);
                YPOSIX(punct);

                YPOSIX(word);
                YPOSIX(endl);
                YPOSIX(dot);
                YPOSIX(core);
                YPOSIX(vowel);
                YPOSIX(consonant);
                pdb.tree.gv("posix.dot");
                ios::vizible::render("posix.dot");
            
        }

        pattern * posix::create(const string &id)
        {
            static const tags::pattern_factory & db = tags::instance().pdb;
            const tags::pattern_creator         *mk = db.search(id);
            if(!mk) throw exception("no posix::%s",id());
            return (*mk)();
        }
        
        pattern * posix::create(const char *s)
        {
            const string _(s);
            return create(_);
        }


        void    posix:: collect(sequence<string> &keys)
        {
            static const tags::pattern_factory & db = tags::instance().pdb;
            kernel::tree_path<uint8_t> path( db.tree.depth(), as_capacity );
            for(tags::pattern_factory::const_iterator it = db.begin();it!=db.end();++it)
            {
                (it.operator->()).encode(path);
                const string tmp((const char *)path(),path.size());
                keys.push_back(tmp);
            }
        }
        
    }

}

