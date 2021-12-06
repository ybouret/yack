//! \file

#ifndef YACK_DATA_FACTORY_INCLUDED
#define YACK_DATA_FACTORY_INCLUDED 1

#include "yack/associative/suffix/map.hpp"
#include <typeinfo>

namespace yack
{

    namespace kernel
    {
        //______________________________________________________________________
        //
        //
        //! base class for common factory methods
        //
        //______________________________________________________________________
        class factory
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~factory() throw();

        protected:
            explicit factory() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void    throw_multiple_key(const std::type_info &)                  const; //!< rise exception
            size_t  check_valid_length(const char *key, const std::type_info &) const; //!< rise exception
            void    throw_unknown_key(const std::type_info &)                   const; //!< rise exception

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(factory);
        };
    }

    //__________________________________________________________________________
    //
    //
    //! generic factory
    //
    //__________________________________________________________________________
    template <
    typename T,
    typename KEY,
    typename CREATOR>
    class factory : public kernel::factory, public suffix_map<KEY,CREATOR>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(KEY,key_type);             //!< aliases
        typedef suffix_map<KEY,CREATOR> map_type; //!< alias
        using   map_type::insert;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit factory() throw() : kernel::factory(), map_type() {} //!< setup
        inline virtual ~factory() throw() {}                                 //!< cleanup

        //______________________________________________________________________
        //
        // declaration methods
        //______________________________________________________________________

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

        //______________________________________________________________________
        //
        // creation methods
        //______________________________________________________________________

        //! create new object by key
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

