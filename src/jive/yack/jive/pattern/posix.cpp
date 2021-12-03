
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
            p->push_back( lower() );
            p->push_back( upper() );
            return pattern::optimize( p.yield() );
        }

        pattern * posix:: digit()
        {
            return new within('0','9');
        }

        pattern * posix:: alnum()
        {
            auto_ptr<op_or> p = new op_or();
            p->push_back( lower() );
            p->push_back( upper() );
            p->push_back( digit() );
            return pattern::optimize( p.yield() );
        }

        pattern * posix:: word()
        {
            auto_ptr<op_or> p = new op_or();
            p->push_back( lower() );
            p->push_back( upper() );
            p->push_back( digit() );
            p->add(  '_'  );
            return pattern::optimize( p.yield() );
        }

        pattern * posix:: xdigit()
        {
            auto_ptr<op_or> p = new op_or();
            p->push_back( digit() );
            p->add('a','f');
            p->add('A','F');
            return pattern::optimize( p.yield() );
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
            return pattern::optimize( p.yield() );
        }

        pattern * posix:: dot()
        {
            auto_ptr<op_none> p = new op_none();
            __fill_endl(*p);
            return pattern::optimize( p.yield() );
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
