//! \file

#ifndef YACK_READABLE_INCLUDED
#define YACK_READABLE_INCLUDED 1

#include "yack/container/collection.hpp"
#include "yack/type/args.hpp"
#include "yack/comparison.hpp"
#include <iostream>

namespace yack
{
	//__________________________________________________________________________
	//
	//
	//! readable class
	//
	//__________________________________________________________________________
	template <typename T>
	class readable : public virtual collection
	{
	public:
		//______________________________________________________________________
		//
		// types and definitions
		//______________________________________________________________________
		YACK_DECL_ARGS(T, type); //!< aliases

		//______________________________________________________________________
		//
		// interface
		//______________________________________________________________________
		virtual const_type& operator[](const size_t) const noexcept = 0; //!< in [1..size()]

		//______________________________________________________________________
		//
		// methods
		//______________________________________________________________________

		//! default Julia,vector-style output
		inline friend std::ostream& operator<<(std::ostream& os, const readable<T>& self)
		{
			os << '[';
			const size_t n = self.size();
			if (n > 0)
			{
				os << self[1];
				for (size_t i = 2; i <= n; ++i) os << ';' << self[i];
			}
			os << ']';
			return os;
		}

		//! helper to test that two containers are item-wise different
		inline friend bool operator != (const readable& lhs, const readable& rhs)  noexcept
		{
			return comparison::disparity(lhs, rhs);
		}

		//! helper to test that two containers are item-wise equals
		inline friend bool operator == (const readable& lhs, const readable& rhs) noexcept
		{
			return comparison::equality(lhs, rhs);
		}


		//______________________________________________________________________
		//
		// C++
		//______________________________________________________________________
		inline virtual ~readable() noexcept {}
	protected:
		inline explicit readable() noexcept {}

	private:
		YACK_DISABLE_COPY_AND_ASSIGN(readable);

	};

}

#endif
