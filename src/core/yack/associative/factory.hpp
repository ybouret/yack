//! \file

#ifndef YACK_DATA_FACTORY_INCLUDED
#define YACK_DATA_FACTORY_INCLUDED 1

#include "yack/associative/suffix/map.hpp"
#include <typeinfo>

namespace yack
{

    namespace kernel
    {
        class factory
        {
        public:
            virtual ~factory() throw();

        protected:
            explicit factory() throw();
            void   throw_multiple_key(const std::type_info &) const;
            size_t check_valid_length(const char *key, const std::type_info &) const;
            void   throw_unknown_key(const std::type_info &) const;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(factory);
        };
    }

    template <
    typename T,
    typename KEY,
    typename CREATOR>
    class factory : public kernel::factory, public suffix_map<KEY,CREATOR>
    {
    public:
        YACK_DECL_ARGS(KEY,key_type);
        typedef suffix_map<KEY,CREATOR> map_type;
        using   map_type::insert;

        inline explicit factory() throw() : kernel::factory(), map_type() {}
        inline virtual ~factory() throw() {}

        //! declaration
        inline void declare(param_key_type key, CREATOR creator)
        {
            assert(NULL!=creator);
            if(!insert(key,creator)) throw_multiple_key(typeid(T));
        }

        //! declaration
        inline void declare(const char *key, CREATOR creator)
        {
            const size_t len = check_valid_length(key,typeid(T));
            if(!this->tree.insert(creator,key,len)) throw_multiple_key(typeid(T));
        }

        template <typename PARAM>
        T * operator()(const PARAM &key) const
        {
            return get(key)();
        }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(factory);

        inline CREATOR get(const_key_type &key) const
        {
            const CREATOR *ptr = this->search(key);
            if(!ptr) throw_unknown_key(typeid(T));
            return *ptr;
        }

        inline CREATOR get(const char *key) const
        {
            const size_t   len = check_valid_length(key,typeid(T));
            const CREATOR *ptr = this->tree.search(key,len);
            if(!ptr) throw_unknown_key(typeid(T));
            return *ptr;
        }
    };

}

#endif

