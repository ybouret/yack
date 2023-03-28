
//! \file

#ifndef YACK_ASSOCIATIVE_KEY_MAPPER_INCLUDED
#define YACK_ASSOCIATIVE_KEY_MAPPER_INCLUDED 1


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
    class key_map : public kernel::suffix_tree<T,uint8_t>, public kernel::key_map
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
        self_type(), kernel::key_map()
        {}

        //! cleanup
        inline virtual ~key_map() noexcept {}

        //! copy
        inline key_map(const key_map &other) :
        self_type(other),
        kernel::key_map()
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
    template <typename T, typename U>
    class key_mapper : public kernel::key_mapper
    {
    public:
        //______________________________________________________________________
        //
        // definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,t);                //!< aliases
        YACK_DECL_ARGS(U,u);                //!< aliases
        typedef key_map<T,U> fwd_dict_type; //!< alias
        typedef key_map<U,T> rev_dict_type; //!< alias


        typedef typename fwd_dict_type::const_iterator fwd_iterator; //!< alias
        typedef typename rev_dict_type::const_iterator rev_iterator; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup
        inline explicit key_mapper() noexcept :
        kernel::key_mapper(),
        size(0),
        forward(),
        reverse()
        {}

        //! cleanup
        inline virtual ~key_mapper() noexcept {}

        //! copy
        inline key_mapper(const key_mapper &other) :
        kernel::key_mapper(),
        size(other.size),
        forward(other.forward),
        reverse(other.reverse)
        {
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! associate source <=> target
        inline void operator()(param_t source,
                               param_u target) {
            { if( !coerce(forward).insert(source,target)) raise_multiple_source(); }
            try
            { if( !coerce(reverse).insert(target,source)) raise_multiple_target(); }
            catch(...)  { (void) coerce(forward).remove(source); throw; }
            ++coerce(size);
        }

        //! display equivalences
        inline friend std::ostream & operator<<(std::ostream &os, const key_mapper &self)
        {
            assert(self.forward->size==self.reverse->size);
            fwd_iterator fwd = self.forward.begin();
            rev_iterator rev = self.reverse.begin();
            os << "{";
            for(size_t i=self.size;i>0;--i,++fwd,++rev)
            {
                os << ' '  << *rev << ':' << *fwd;
            }
            os << " }";
            return os;
        }

        //! send global source to local target
        template <typename ARR, typename BRR> inline
        void send(ARR &target, BRR &source) const
        {
            fwd_iterator fwd = forward.begin();
            rev_iterator rev = reverse.begin();
            for(size_t i=size;i>0;--i,++fwd,++rev)
            {
                target[ *fwd ] = source[ *rev ];
            }
        }

        //! recv global target from local source
        template <typename ARR, typename BRR> inline
        void recv(ARR &target, BRR &source) const
        {
            fwd_iterator fwd = forward.begin();
            rev_iterator rev = reverse.begin();
            for(size_t i=size;i>0;--i,++fwd,++rev)
            {
                target[ *rev ] = source[ *fwd ];
            }
        }

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const size_t        size;    //!< current common size
        const fwd_dict_type forward; //!< source => target
        const rev_dict_type reverse; //!< target => source

    private:
        YACK_DISABLE_ASSIGN(key_mapper);
    };

}

#endif

