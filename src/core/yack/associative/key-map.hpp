
//! \file

#ifndef YACK_ASSOCIATIVE_KEY_MAP_INCLUDED
#define YACK_ASSOCIATIVE_KEY_MAP_INCLUDED 1


#include "yack/data/suffix/tree.hpp"
#include "yack/container/iterator/linked.hpp"
#include "yack/associative/be-key.hpp"

namespace yack
{

    namespace kernel
    {
        //______________________________________________________________________
        //
        //! base class for key_map, to raise errors
        //______________________________________________________________________
        class key_map
        {
        public:
            virtual ~key_map() noexcept;        //!< cleanup
        protected:
            explicit key_map() noexcept;        //!< setup
            void     raise_missing_key() const; //!< throw

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(key_map);
        };
    }
    //__________________________________________________________________________
    //
    //
    //! key map to encode integral mapping in compact trees
    //
    //__________________________________________________________________________
    template <typename KEY, typename T>
    class key_map : public kernel::key_map, public kernel::suffix_tree<T,uint8_t>
    {
    public:
        //______________________________________________________________________
        //
        // definitions
        //______________________________________________________________________
        typedef kernel::suffix_tree<T,uint8_t> self_type;  //!< alias
        typedef be_key<KEY>                    bkey_type;  //!< alias
        typedef typename self_type::knot_type  knot_type;  //!< alias
        typedef typename self_type::type       type;       //!< alias
        typedef typename self_type::const_type const_type; //!< alias
        typedef typename self_type::param_type param_type; //!< alias
        YACK_DECL_ARGS(KEY,key_type);                      //!< aliases

        //! alias, const only
        typedef iterating::linked<const_type,const knot_type,iterating::forward> const_iterator;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        //! setup empty
        inline explicit key_map() noexcept :
        kernel::key_map(), self_type()
        {}

        //! cleanup
        inline virtual ~key_map() noexcept {}

        //! copy
        inline key_map(const key_map &other) :
        kernel::key_map(),
        self_type(other)
        {}

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! use big-endian key to have a compact interal tree
        inline bool insert(param_key_type k, param_type v)
        {
            const bkey_type key(k);
            return self_type::insert(v,key.begin(),key.measure());
        }

        //! search existing value
        inline const_type *search(param_key_type k) const noexcept
        {
            const bkey_type key(k);
            return self_type::search(key.begin(),key.measure());
        }

        //! remove value
        inline bool remove(param_key_type k) noexcept
        {
            const bkey_type key(k);
            return self_type::remove(key.begin(),key.measure());
        }

        //! access
        const_type & operator[](const_key_type k) const {
            const_type *p = search(k);
            if(!p) raise_missing_key();
            return *p;
        }

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

        //______________________________________________________________________
        //
        // iterators
        //______________________________________________________________________

        const_iterator begin() const noexcept { return (**this).head; } //!< forward iterator begin
        const_iterator end()   const noexcept { return NULL; }          //!< forward iterator end



    private:
        YACK_DISABLE_ASSIGN(key_map);
    };

}

#endif

