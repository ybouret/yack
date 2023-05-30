//! \file

#ifndef YACK_IPSO_API_INCLUDED
#define YACK_IPSO_API_INCLUDED 1

#include "yack/ipso/inside.hpp"
#include "yack/large-object.hpp"
#include "yack/type/args.hpp"
#include "yack/container.hpp"
#include "yack/system/rtti.hpp"

namespace yack
{
    namespace ipso
    {

        //__________________________________________________________________
        //
        //
        //! common API class
        //
        //__________________________________________________________________
        class api_ : public large_object
        {
        public:
            static const size_t width = 23;   //!< to display
            virtual ~api_() noexcept;   //!< cleanup
            void     ldz()  noexcept;   //!< free
            void     ldz(const size_t); //!< resume(n)

        protected:
            explicit    api_(container &) noexcept; //!< setup
            container &super;                       //!< original container

            //! helper for RTTI
            static void disp(const rtti &outer,
                             const rtti &inner);
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(api_);
        };


        //__________________________________________________________________
        //
        //
        //! generic API for a given type
        //
        //__________________________________________________________________
        template <typename T>
        class api : public api_
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            YACK_DECL_ARGS(T,type);                        //!< aliases
            typedef typename inside<T>::type inside_type;  //!< alias

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual void        append(const inside_type &) = 0; //!< append an inside type
            virtual inside_type reduce() = 0;                    //!< reduce to inside type

            //__________________________________________________________________
            //
            // non-virtual interface
            //__________________________________________________________________

            //! high-level convert and push value
            inline api & operator<<(param_type args) {
                append( inside<type>::send(args) );
                return *this;
            }

            //! high-level reduction and conversion
            inline mutable_type operator*() {
                const inside_type res = reduce();
                return inside<T>::recv(res);
            }

            

            //! reduce one arg
            inline inside_type operator()(param_type a) { ldz(1); *this << a; return reduce(); }

            //! reduce two args
            inline inside_type operator()(param_type a,
                                          param_type b) { ldz(2); *this << a << b; return reduce(); }

            //! reduce three args
            inline inside_type operator()(param_type a,
                                          param_type b,
                                          param_type c) { ldz(3); *this << a << b << c; return reduce(); }

            //! reduce four args
            inline inside_type operator()(param_type a,
                                          param_type b,
                                          param_type c,
                                          param_type d) { ldz(4); *this << a << b << c << d; return reduce(); }


            //! load a range, no ldz
            template <typename ITERATOR>
            inline api & range(ITERATOR curr, size_t n) {
                while(n-- > 0) {
                    (*this) << *(curr++);
                }
                return *this;
            }

            //! load a sequence, no ldz
            template <typename SEQUENCE>
            inline api & range(SEQUENCE &seq) {
                return range( seq.begin(), seq.size() );
            }

            //! load a tableau, no ldz
            template <typename TABLEAU>
            inline api & tableau(TABLEAU &arr) {
                for(size_t i=arr.size();i>0;--i)
                {
                    (*this) << arr[i];
                }
                return *this;
            }

            //! display info
            inline  static void display()
            {
                static const rtti &myID = rtti::use<type>();
                static const rtti &inID = rtti::use<inside_type>();
                disp(myID,inID);
            }

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! cleanup
            inline virtual ~api() noexcept {}

        protected:
            //! setup
            inline explicit api(container &_) noexcept : api_(_) {}

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(api);
        };


    }

}

#endif

