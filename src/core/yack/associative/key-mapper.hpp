
//! \file

#ifndef YACK_ASSOCIATIVE_KEY_MAPPER_INCLUDED
#define YACK_ASSOCIATIVE_KEY_MAPPER_INCLUDED 1


#include "yack/data/suffix/tree.hpp"
#include "yack/container/iterator/linked.hpp"
#include "yack/associative/be-key.hpp"
namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! key map to encode integral mapping in compact trees
    //
    //__________________________________________________________________________
    template <typename T>
    class key_map : public kernel::suffix_tree<T,uint8_t>
    {
    public:
        //______________________________________________________________________
        //
        // definitions
        //______________________________________________________________________
        typedef kernel::suffix_tree<T,uint8_t> self_type;  //!< alias
        typedef be_key<T>                      bkey_type;  //!< alias
        typedef typename self_type::knot_type  knot_type;  //!< alias
        typedef typename self_type::type       type;       //!< alias
        typedef typename self_type::const_type const_type; //!< alias
        typedef typename self_type::param_type param_type; //!< alias

        
        //! alias, const only
        typedef iterating::linked<const_type,const knot_type,iterating::forward> const_iterator;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit key_map() noexcept : self_type() {}        //!< setup empty
        inline virtual ~key_map() noexcept {}                      //!< cleanup
        inline key_map(const key_map &other) : self_type(other) {} //!< copy

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! use big-endian key to have a compact interal tree
        inline bool insert(param_type k, param_type v)
        {
            const bkey_type key(k);
            return self_type::insert(v,key.begin(),key.measure());
        }

        //! search existing value
        inline const_type *search(param_type k)
        {
            const bkey_type key(k);
            return self_type::search(key.begin(),key.measure());
        }

        //! remove value
        inline bool remove(param_type k) noexcept
        {
            const bkey_type key(k);
            return self_type::remove(key.begin(),key.measure());
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

    namespace kernel
    {
        //______________________________________________________________________
        //
        //! base class for key mapper, to raise errors
        //______________________________________________________________________
        class key_mapper
        {
        public:
            virtual ~key_mapper() noexcept; //!< cleanup

        protected:
            explicit key_mapper() noexcept; //!< setup

            void     raise_multiple_source() const; //!< throw
            void     raise_multiple_target() const; //!< throw

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(key_mapper);
        };
    }

    //__________________________________________________________________________
    //
    //
    //! forward/reverse bijective key mapping
    //
    //__________________________________________________________________________
    template <typename T>
    class key_mapper : public kernel::key_mapper
    {
    public:
        //______________________________________________________________________
        //
        // definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);       //!< aliases
        typedef key_map<T> dict_type; //!< aliases
        typedef typename dict_type::const_iterator const_iterator;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup
        inline explicit key_mapper() noexcept :
        kernel::key_mapper(),
        forward(),
        reverse()
        {}

        //! cleanup
        inline virtual ~key_mapper() noexcept {}

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! associate source <=> target
        inline void operator()(param_type source,
                               param_type target) {
            { if( !coerce(forward).insert(source,target)) raise_multiple_source(); }
            try
            { if( !coerce(reverse).insert(target,source)) raise_multiple_target(); }
            catch(...)  { (void) coerce(forward).remove(source); throw; }
        }

        //! display equivalences
        inline friend std::ostream & operator<<(std::ostream &os, const key_mapper &self)
        {
            assert(self.forward->size==self.reverse->size);
            const_iterator fwd = self.forward.begin();
            const_iterator rev = self.reverse.begin();
            os << "{";
            for(size_t i=self.forward->size;i>0;--i,++fwd,++rev)
            {
                os << ' '  << *fwd << ':' << *rev;
            }
            os << " }";
            return os;
        }

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const dict_type forward; //!< source => target
        const dict_type reverse; //!< target => source

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(key_mapper);
    };

}

#endif

