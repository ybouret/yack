
#include "yack/jive/pattern/posix.hpp"

namespace yack
{
    namespace jive
    {
#if 0
        Pattern * posix:: lower()
        {
            return Range::Create('a','z');
        }

        Pattern * posix:: upper()
        {
            return Range::Create('A','Z');
        }

        Pattern * posix:: alpha()
        {
            auto_ptr<Or> p = Or::Create();
            p->push_back( lower() );
            p->push_back( upper() );
            p->rework();
            return p.yield();
        }

        Pattern * posix:: digit()
        {
            return Range::Create('0','9');
        }

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
