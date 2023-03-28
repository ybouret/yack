
//! \file

#ifndef YACK_ASSOCIATIVE_KEY_MAPPER_INCLUDED
#define YACK_ASSOCIATIVE_KEY_MAPPER_INCLUDED 1


#include "yack/associative/key-map.hpp"

namespace yack
{

    
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

