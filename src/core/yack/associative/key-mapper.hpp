
//! \file

#ifndef YACK_ASSOCIATIVE_KEY_MAPPER_INCLUDED
#define YACK_ASSOCIATIVE_KEY_MAPPER_INCLUDED 1


#include "yack/data/suffix/tree.hpp"
#include "yack/container/iterator/linked.hpp"
#include "yack/associative/be-key.hpp"
namespace yack
{

    template <typename T>
    class key_map : public kernel::suffix_tree<T,uint8_t>
    {
    public:
        typedef kernel::suffix_tree<T,uint8_t> self_type;  //!< alias
        typedef be_key<T>                      bkey_type;  //!< alias
        typedef typename self_type::knot_type  knot_type;  //!< alias
        typedef typename self_type::type       type;       //!< alias
        typedef typename self_type::const_type const_type; //!< alias
        typedef typename self_type::param_type param_type; //!< alias

        //! alias, const only
        typedef iterating::linked<const_type,const knot_type,iterating::forward> const_iterator;

        inline explicit key_map() noexcept : self_type() {}
        inline virtual ~key_map() noexcept {}
        inline key_map(const key_map &other) : self_type(other) {}


        //! use big-endian key to have a compact interal tree
        inline bool insert(param_type k, param_type v)
        {
            const bkey_type key(k);
            return self_type::insert(v,key.begin(),key.measure());
        }

        inline const_type *search(param_type k)
        {
            const bkey_type key(k);
            return self_type::search(key.begin(),key.measure());
        }

        inline bool remove(param_type k) noexcept
        {
            const bkey_type key(k);
            return self_type::remove(key.begin(),key.measure());
        }


        const_iterator begin() const noexcept { return (**this).head; } //!< forward iterator begin
        const_iterator end()   const noexcept { return NULL; }          //!< forward iterator end

        //! display content
        inline friend std::ostream & operator<<(std::ostream &os, const key_map &self)
        {
            os << "{ ";
            const knot_type *knot = (*self).head;
            if(knot) {
                os << **knot;
                for(knot=knot->next;knot;knot=knot->next)
                {
                    os << ", " << **knot;
                }
            }
            os << " }";
            return os;
        }

    private:
        YACK_DISABLE_ASSIGN(key_map);
    };

    namespace kernel
    {
        class key_mapper
        {
        public:
            virtual ~key_mapper() noexcept;

        protected:
            explicit key_mapper() noexcept;
            void     raise_multiple_source() const;
            void     raise_multiple_target() const;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(key_mapper);
        };
    }

    template <typename T>
    class key_mapper : public kernel::key_mapper
    {
    public:
        YACK_DECL_ARGS(T,type);
        typedef key_map<T> dict_type;

        inline explicit key_mapper() noexcept {}
        inline virtual ~key_mapper() noexcept {}

        inline void operator()(param_type source,
                               param_type target) {
            { if( !coerce(forward).insert(source,target)) raise_multiple_source(); }
            try
            { if( !coerce(reverse).insert(target,source)) raise_multiple_target(); }
            catch(...)  { (void) coerce(forward).remove(source); throw; }
        }

        const dict_type forward;
        const dict_type reverse;

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(key_mapper);
    };

}

#endif

