
#include "yack/ios/ascii/converter.hpp"
#include "yack/ios/ascii/convert.hpp"
#include "yack/system/rtti.hpp"
#include "yack/associative/be-addr.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace ios
    {
        namespace ascii
        {

            const char converter:: call_sign[] = "ascii::converter";

            converter:: ~converter() throw()
            {
            }

            template <typename T>
            static inline
            void to_intg(void        *target,
                         const char  *buff,
                         const size_t size,
                         const char  *when)
            {
                assert(target);
                assert(buff);
                *static_cast<T*>(target) = convert::to<T>(buff,size,when);
            }




            template <typename T>
            static inline void insert_intg(converter::tree_type &tree)
            {
                const be_address key( rtti::use<T>() );
                (void) tree.insert( to_intg<T>, key.begin(), key.measure() );
            }


            template <typename T>
            static inline
            void to_real(void        *target,
                         const char  *buff,
                         const size_t size,
                         const char  *when)
            {
                assert(target);
                assert(buff);
                *static_cast<T*>(target) = convert::real<T>(buff,size,when);
            }

            template <typename T>
            static inline void insert_real(converter::tree_type &tree)
            {
                const rtti      &tid = rtti::use<T>();
                const be_address key( tid );
                if(! tree.insert(to_real<T>,key.begin(),key.measure()))
                    throw exception("cannot insert loader for <%s>", tid.name()() );

            }

            static inline void to_str(void *target,const char *buff, const size_t size, const char *)
            {
                assert(target);
                *static_cast<string*>(target) = string(buff,size);
            }

            static inline void insert_str(converter::tree_type &tree)
            {
                const rtti      &tid = rtti::use<string>();
                const be_address key( tid );
                if(! tree.insert(to_str,key.begin(),key.measure()))
                    throw exception("cannot insert loader for <%s>", tid.name()() );
            }


            converter:: converter() : singleton<converter>()
            {
                insert_intg<int8_t>(tree);
                insert_intg<int16_t>(tree);
                insert_intg<int32_t>(tree);
                insert_intg<int64_t>(tree);

                insert_intg<uint8_t>(tree);
                insert_intg<uint16_t>(tree);
                insert_intg<uint32_t>(tree);
                insert_intg<uint64_t>(tree);

                insert_intg<char>(tree);
                insert_intg<short>(tree);
                insert_intg<int>(tree);
                insert_intg<long>(tree);
                insert_intg<long long>(tree);

                insert_intg<unsigned char>(tree);
                insert_intg<unsigned short>(tree);
                insert_intg<unsigned int>(tree);
                insert_intg<unsigned long>(tree);
                insert_intg<unsigned long long>(tree);

                insert_intg<size_t>(tree);
                insert_intg<ptrdiff_t>(tree);

                insert_real<float>(tree);
                insert_real<double>(tree);
                insert_real<long double>(tree);

                insert_str(tree);

                tree.gv("ascii-loader.dot");
                ios::vizible::render("ascii-loader.dot");

            }

            converter::builder & converter::operator[](const rtti &tid) const
            {
                const be_address key(tid);
                const builder   *hfn = tree.search(key.begin(),key.measure());
                if(!hfn) throw exception("no %s[%s]",call_sign,tid.name()());
                return coerce(*hfn);
            }
            
        }
    }
}

