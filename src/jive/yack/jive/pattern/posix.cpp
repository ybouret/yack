
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

#if 0
        Pattern * posix:: alnum()
        {
            auto_ptr<Or> p = Or::Create();
            p->push_back( lower() );
            p->push_back( upper() );
            p->push_back( digit() );
            p->rework();
            return p.yield();
        }

        Pattern * posix:: word()
        {
            auto_ptr<Or> p = Or::Create();
            p->push_back( lower() );
            p->push_back( upper() );
            p->push_back( digit() );
            p->add(  '_'  );
            p->rework();
            return p.yield();
        }

        Pattern * posix:: xdigit()
        {
            auto_ptr<Or> p = Or::Create();
            p->push_back( digit() );
            p->add('a','f');
            p->add('A','F');
            p->rework();
            return p.yield();
        }

        Pattern * posix:: blank()
        {
            return Logical::Among(" \t");
        }

        Pattern * posix:: space()
        {
            return Logical::Among(" \t\n\r\v\f");
        }

        Pattern * posix::punct()
        {
            return Logical::Among("][!\"#$%&'()*+,./:;<=>?@\\^_`{|}~-");
        }

        static inline void __fill_endl( Pattern::List &ops )
        {
            ops.push_back( Single::Create('\n') );
            ops.push_back( Single::Create('\r') );
            ops.push_back( Logical::Equal("\r\n") );
        }

        Pattern * posix:: endl()
        {
            auto_ptr<Or> p = Or::Create();
            __fill_endl(*p);
            p->rework();
            return p.yield();
        }

        Pattern * posix:: dot()
        {
            auto_ptr<None> p = None::Create();
            __fill_endl(*p);
            p->rework();
            return p.yield();
        }

        Pattern * posix:: core()
        {
            auto_ptr<Or> p = Or::Create();
            p->add( 0x20 );
            p->add( 0x21 );
            p->add( 0x23,0x26 );
            p->add( 0x28,0x5B );
            p->add( 0x5D,0x7F );
            p->rework();
            return p.yield();
        }

        static const char __vowels__[] = "aeiouyAEIOUY";

        Pattern * posix:: vowel()
        {
            return Logical::Among(__vowels__);
        }

        Pattern * posix:: consonant()
        {
            return Logical::Avoid(__vowels__);
        }
#endif
    }

}
