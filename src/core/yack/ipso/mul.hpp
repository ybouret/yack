//! \file

#ifndef YACK_IPSO_MUL_INCLUDED
#define YACK_IPSO_MUL_INCLUDED 1

#include "yack/ipso/api.hpp"
#include "yack/ordered/roster.hpp"
#include "yack/large-object.hpp"
#include "yack/counted.hpp"

namespace yack
{
    namespace ipso
    {

        //______________________________________________________________________
        //
        //! wrapper to call inside<T>::comp
        //______________________________________________________________________
        template <typename T> struct inside_comparator
        {
            typedef typename inside<T>::type inside_type; //!< alias

            //! forwarding call
            inline sign_type operator()(const inside_type &l,
                                        const inside_type &r) noexcept
            {
                return inside<T>::comp(l,r);
            }
        };


        //______________________________________________________________________
        //
        //
        //! multiplication
        //
        //______________________________________________________________________
        template <typename T>
        class mul : public large_object, public counted, public roster< typename inside<T>::type, inside_comparator<T> >, public api<T>
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            YACK_DECL_ARGS(T,type);                              //!< aliases
            typedef typename inside<T>::type        inside_type; //!< alias
            typedef inside_comparator<type>         incomp_type; //!< alias
            typedef roster<inside_type,incomp_type> proto_class; //!< alias

            using proto_class::size;
            using proto_class::lower;
            using proto_class::upper;
            using api<T>::positive1;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline explicit mul() noexcept       : proto_class(), api<T>( static_cast<container&>(*this) )  {} //!< setup empty
            inline explicit mul(const size_t n)  : proto_class(n), api<T>( static_cast<container&>(*this) ) {} //!< setup with capacity
            inline virtual ~mul() noexcept {} //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! append using inside_type comparison
            inline virtual void append(const inside_type &args) { this->insert(args); }

            //! unsigned power function
            inline void upower(const inside_type args, int n)
            {
                if(n) this->insert(args,n); else this->insert(positive1);
            }

            //! power function, using multiple inserts
            inline void power(const inside_type args, int n)
            {
                switch( __sign::of(n) )
                {
                    case __zero__:
                        this->insert(positive1);
                        break;

                    case positive:
                        this->insert(args,static_cast<unsigned>(n));
                        return;

                    case negative: {
                        const inside_type _I = positive1/args;
                        this->insert(_I,static_cast<unsigned>(-n));
                    } break;
                }
            }

            //! reduce algorithm
            inline virtual inside_type reduce()
            {
                switch( size() )
                {
                    case 0: { const inside_type res(0); return res; }
                    case 1: return lower();
                    default:
                        break;
                }

                assert(size()>=2);

            REDUCE:
                const inside_type lhs = lower();
                const inside_type rhs = upper();
                const inside_type res = lhs*rhs;
                if(size()<=0) return res;
                this->insert(res);
                goto REDUCE;
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(mul);
        };
    }

}

#endif


