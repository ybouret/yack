//! \file

#ifndef YACK_CAMEO_MUL_INCLUDED
#define YACK_CAMEO_MUL_INCLUDED 1

#include "yack/cameo/def.hpp"
#include "yack/ordered/list.hpp"

namespace yack
{
	namespace cameo
	{
		template <typename T>
		class tagged_real
		{
		public:
			tagged_real(const T) noexcept;
			~tagged_real()       noexcept;
			tagged_real(const tagged_real&) noexcept;

			inline friend bool operator<(const tagged_real& lhs, const tagged_real& rhs) noexcept
			{
				return lhs.exponent < rhs.exponent;
			}

			inline friend std::ostream& operator<<(std::ostream& os, const tagged_real& self)
			{
				os << self.value << '[' << self.exponent << ']';
				return os;
			}

			const T   value;
			const int exponent;

		private:
			YACK_DISABLE_ASSIGN(tagged_real);
		};

		template <typename T>
		class mul : public object_type, public sorted_list< tagged_real<T> >
		{
		public:
			typedef tagged_real<T>    type;
			typedef sorted_list<type> self_type;

			explicit mul() noexcept;
			virtual ~mul() noexcept;
			explicit mul(const size_t);

			mul& operator=(const T init);
			mul& operator<<(const T args);

			void push(const T args);
			void push(const T args, const size_t n);
			void upower(const T x, const size_t n);
			void ipower(const T x, const ptrdiff_t i);

			T product();


		private:
			YACK_DISABLE_COPY_AND_ASSIGN(mul);

		};
	}

}

#endif
