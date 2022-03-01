
#include "yack/ios/scribe/repository.hpp"
#include "yack/ios/scribe/primary.hpp"
#include "yack/associative/be-addr.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace ios
    {

        namespace {


            class char_scribe : public  scribe
            {
            public:
                inline explicit char_scribe() :  scribe( rtti::use<char>() ) {}
                inline virtual ~char_scribe() throw() {}

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(char_scribe);
                inline virtual string to_string(const void *addr) const
                {
                    assert(addr);
                    const string tmp = *(const char *)addr;
                    return tmp;
                }
            };

            class format_scribe : public scribe
            {
            public:
                const string &format;

                inline explicit format_scribe(const rtti   &uid,
                                              const string &fmt) throw() :
                scribe(uid),
                format(fmt)
                {
                }

                inline virtual ~format_scribe() throw()
                {
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(format_scribe);
            };

            template <typename T>
            class real_scribe : public format_scribe
            {
            public:
                inline explicit real_scribe(const string &fmt) :
                format_scribe( rtti::use<T>(), fmt )
                {
                }

                inline virtual ~real_scribe() throw()
                {
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(real_scribe);
                inline virtual string to_string(const void *addr) const
                {
                    assert(addr);
                    return vformat( format(), static_cast<double>(*(const T*)addr) );
                }
            };


        }

        const char scribes:: call_sign[] = "ios::scribes";

        scribes:: ~scribes() throw()
        {
        }

        
        scribes:: scribes() :
        real_format("%.15g"),
        db()
        {
            use( new char_scribe() );
            use( new real_scribe<float>(real_format) );
            use( new real_scribe<double>(real_format) );
            use( new real_scribe<long double>(real_format) );
            
        }


        void scribes:: use(scribe *s)
        {
            assert(s);
            const scribe_ptr ptr(s);
            const be_address key(ptr->tid);
            if(!db.insert(ptr,key.begin(),key.measure()))
                throw exception("%s multiple <%s>", call_sign, ptr->tid.name()());
        }

        

        
    }

}

