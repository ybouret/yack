namespace yack
{

	namespace cameo
	{
		template <>
		tagged_real<real_t>:: ~tagged_real() noexcept
		{

		}

		static inline int exponent_of(const real_t args) noexcept
		{
			int ex = 0;
			(void)std::frexp(args, &ex);
			return ex;
		}
		template <>
		tagged_real<real_t>::tagged_real(const real_t args) noexcept :
			value(args),
			exponent(exponent_of(args))
		{

		}

		template <>
		tagged_real<real_t>::tagged_real(const tagged_real& other) noexcept :
			value(other.value),
			exponent(other.exponent)
		{

		}

	}

}

namespace yack
{

	namespace cameo
	{
		template <>
		mul<real_t>::mul() noexcept : object_type(), self_type() {}

		template <>
		mul<real_t>::mul(const size_t n) : object_type(), self_type(n) {}

		template <>
		mul<real_t>:: ~mul() noexcept {}

		template <>
		void mul<real_t>::push(const real_t args)
		{
			const type x(args);
			insert(x);
		}

		template <>
		void mul<real_t>::push(const real_t args, const size_t n)
		{
			const type x(args);
			insert(x, n);
		}

		template <>
		mul<real_t>& mul<real_t>:: operator=(const real_t init)
		{
			free();
			push(init);
			return *this;
		}

		template <>
		mul<real_t>& mul<real_t>:: operator<<(const real_t args)
		{
			push(args);
			return *this;
		}

		template <>
		void mul<real_t>::upower(const real_t args, const size_t n)
		{
			const type x(args);
			insert(x, n);
		}

		template <>
		void mul<real_t>::ipower(const real_t args, const ptrdiff_t n)
		{
			if (n < 0)
			{
				static const real_t one(1);
				const type x(one / args);
				insert(x, static_cast<size_t>(-n));
			}
			else
			{
				if (n > 0)
				{
					const type x(args);
					insert(x, static_cast<size_t>(n));
				}
			}
		}


		template <>
		real_t mul<real_t>::product()
		{
			switch (size())
			{
			case 0:
				return 0;

			case 1:
				return pull_back().value;

			default:
				break;
			}
			assert(size() >= 2);
		REDUCE:
			const real_t l = pull_front().value; assert(size() >= 1);
			const real_t r = pull_back().value;
			const real_t p = l * r;
			if (size() <= 0) return p;
			push(p);
			goto REDUCE;
		}
	}

}