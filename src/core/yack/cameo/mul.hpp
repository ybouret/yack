//! \file

#ifndef YACK_CAMEO_MUL_INCLUDED
#define YACK_CAMEO_MUL_INCLUDED 1

#include "yack/cameo/def.hpp"
#include "yack/ordered/list.hpp"

namespace yack
{
	namespace cameo
	{

        //______________________________________________________________________
        //
        //
        //! a real number and its expibebt
        //
        //______________________________________________________________________
		template <typename T>
		class tagged_real
		{
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
			tagged_real(const T) noexcept;            //!< setup
			~tagged_real()       noexcept;            //!< cleanup
			tagged_real(const tagged_real&) noexcept; //!< copy

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! comparator for sorted_list
			inline friend bool operator<(const tagged_real& lhs, const tagged_real& rhs) noexcept
			{
				return lhs.exponent < rhs.exponent;
			}

            //! display to debug
			inline friend std::ostream& operator<<(std::ostream& os, const tagged_real& self)
			{
				os << self.value << '[' << self.exponent << ']';
				return os;
			}

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
			const T   value;      //!< original value
			const int exponent;   //!< frexp(value,&exponent)

		private:
			YACK_DISABLE_ASSIGN(tagged_real);
		};

        //______________________________________________________________________
        //
        //
        //! multiplier
        //
        //______________________________________________________________________
		template <typename T>
		class mul : public object_type, public sorted_list< tagged_real<T> >
		{
		public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
			typedef tagged_real<T>    type;      //!< alias
			typedef sorted_list<type> self_type; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
			explicit mul() noexcept;       //!< setup   empty
			virtual ~mul() noexcept;       //!< cleanup
			explicit mul(const size_t);    //!< setup with capacity

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void push(const T args);                   //!< append args
            void push(const T args, const size_t n);   //!< append args^n
            void ipower(const T x, const ptrdiff_t i); //!< append args&i
            T    product();                            //!< compute product

            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________
            mul& operator=(const T init);    //!< initialize
			mul& operator<<(const T args);   //!< append a new multiplicand





		private:
			YACK_DISABLE_COPY_AND_ASSIGN(mul);

		};
	}

}

#endif
