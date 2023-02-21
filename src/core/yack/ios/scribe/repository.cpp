
#include "yack/ios/scribe/repository.hpp"
#include "yack/associative/be-addr.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace ios
    {

        namespace {

            class bool_scribe : public scribe
            {
            public:
                inline explicit bool_scribe() :  scribe( rtti::use<bool>() ) {}
                inline virtual ~bool_scribe() noexcept {}



            private:
                YACK_DISABLE_COPY_AND_ASSIGN(bool_scribe);
                inline virtual string to_string(const void *addr) const
                {
                    assert(addr);
                    const bool flag = *(const bool *)addr;
                    if(flag)
                    {
                        return string("true");
                    }
                    else
                    {
                        return string("false");
                    }

                }
            };


            class char_scribe : public  scribe
            {
            public:
                inline explicit char_scribe() :  scribe( rtti::use<char>() ) {}
                inline virtual ~char_scribe() noexcept {}

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
                                              const string &fmt) noexcept :
                scribe(uid),
                format(fmt)
                {
                }

                inline virtual ~format_scribe() noexcept
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

                inline virtual ~real_scribe() noexcept
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

            template <typename T>
            class u_scribe : public scribe
            {
            public:
                inline explicit u_scribe()  : scribe( rtti::use<T>() ) {}
                inline virtual ~u_scribe() noexcept {}

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(u_scribe);
                inline virtual string to_string(const void *addr) const
                {
                    assert(addr);
                    return vformat( "%lu", static_cast<unsigned long>(*(const T*)addr) );
                }
            };


            template <typename T>
            class s_scribe : public scribe
            {
            public:
                inline explicit s_scribe()  : scribe( rtti::use<T>() ) {}
                inline virtual ~s_scribe() noexcept {}

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(s_scribe);
                inline virtual string to_string(const void *addr) const
                {
                    assert(addr);
                    return vformat( "%ld", static_cast<long>(*(const T*)addr) );
                }
            };


        }

        const char scribes:: call_sign[] = "ios::scribes";

        scribes:: ~scribes() noexcept
        {
        }


#define YACK_USCRIBE(TYPE) chk( new u_scribe<TYPE>() )
#define YACK_SSCRIBE(TYPE) chk( new s_scribe<TYPE>() )

        scribes:: scribes() :
        singleton<scribes>(),
        real_format("%.15g"),
        db()
        {
            use( new bool_scribe() );
            use( new char_scribe() );
            use( new real_scribe<float>(real_format) );
            use( new real_scribe<double>(real_format) );
            use( new real_scribe<long double>(real_format) );

            YACK_USCRIBE(unsigned char);
            YACK_USCRIBE(unsigned short);
            YACK_USCRIBE(unsigned int);
            YACK_USCRIBE(unsigned long);

            YACK_USCRIBE(uint8_t);
            YACK_USCRIBE(uint16_t);
            YACK_USCRIBE(uint32_t);
            YACK_USCRIBE(uint64_t);

            YACK_SSCRIBE(char);
            YACK_SSCRIBE(short);
            YACK_SSCRIBE(int);
            YACK_SSCRIBE(long);

            YACK_SSCRIBE(int8_t);
            YACK_SSCRIBE(int16_t);
            YACK_SSCRIBE(int32_t);
            YACK_SSCRIBE(int64_t);

        }


        void scribes:: use(scribe *s)
        {
            assert(s);
            const scribe_ptr ptr(s);
            const be_address key(ptr->tid);
            if(!db.insert(ptr,key.begin(),key.measure()))
                throw exception("%s multiple <%s>", call_sign, ptr->tid.name()());
        }


        void scribes:: chk(scribe *s)
        {
            assert(s);
            const scribe_ptr ptr(s);
            const be_address key(ptr->tid);
            if(!db.insert(ptr,key.begin(),key.measure()))
            {

            }
        }

        const scribe & scribes:: get(const std::type_info &tid) const
        {
            const rtti       &uid = rtti::use(tid);
            const be_address  key(uid);
            const scribe_ptr *pps = db.search(key.begin(),key.measure());
            if(!pps)
                throw exception("%s no <%s>",call_sign,uid.name()());
            return **pps;
        }



        
    }

}

