//! \file

#ifndef YACK_CAMEO_ADD_INCLUDED
#define YACK_CAMEO_ADD_INCLUDED 1

#include "yack/cameo/def.hpp"
#include "yack/ordered/pqueue.hpp"
#include "yack/math/api.hpp"

namespace yack
{
	namespace cameo
	{
		//______________________________________________________________________
		//
		//
		//! type comparator for add
		//
		//______________________________________________________________________
		template <typename T> struct add_comparator
		{
			//! decreasing absolute value
			inline int operator()(const T& lhs, const T& rhs) const
			{
				const typename scalar_for<T>::type L = math::abs_of(lhs);
				const typename scalar_for<T>::type R = math::abs_of(rhs);
				return (L < R) ? 1 : ((R < L) ? -1 : 0);
			}
		};

		//______________________________________________________________________
		//
		//
		//! addition with priority queue
		//
		//______________________________________________________________________
		template <typename T>
		class add : public object_type, public pqueue<T, add_comparator<T>, allocator_type>
		{
		public:
			//__________________________________________________________________
			//
			// types
			//__________________________________________________________________
			YACK_DECL_ARGS(T, type);                                        //!< aliases
			typedef pqueue<T, add_comparator<T>, allocator_type> self_type; //!< alias

			using self_type::size;
			using self_type::pull;
			using self_type::push_;
			using self_type::resume;

			//__________________________________________________________________
			//
			// C++
			//__________________________________________________________________
			inline explicit add() noexcept : object_type(), self_type() {}                    //!< setup empty
			inline virtual ~add() noexcept {}                                                 //!< cleanup
			inline explicit add(const size_t n) : object_type(), self_type(n, as_capacity) {} //!< setup with capacity

			//__________________________________________________________________
			//
			// methods
			//__________________________________________________________________

            //! get the current sum
			inline T sum() {
				switch (size())
				{
				case 0:  return 0;
				case 1:  return pull();
				default: break;
				}
				assert(size() > 1);
			REDUCE:
				const_type lhs = pull(); assert(size() >= 1);
				const_type rhs = pull();
				const_type tmp = lhs + rhs;
				if (size() <= 0) return tmp;
				push_(tmp);
				goto REDUCE;
			}

            //! sum of a range
			template <typename ITERATOR>
			inline T range(ITERATOR i, size_t n)
			{
				resume(n);
				while (n-- > 0) push_(*(i++));
				return sum();
			}

            //! sum of a sequence
			template <typename SEQUENCE>
			inline T range(SEQUENCE& seq)
			{
				return range(seq.begin(), seq.size());
			}

            //! sum of a C++ array
			template <typename ARR>
			inline T tableau(ARR& arr)
			{
				size_t n = arr.size();
				resume(n);
				while (n > 0) push_(arr[n--]);
				return sum();
			}

            //! sum of a C array
			template <typename U>
			inline T tableau(U arr[], const size_t num)
			{
				assert(yack_good(arr, num));
				return range(arr, num);
			}

            //! dot product
			template <typename LHS, typename RHS>
			inline T dot(LHS& lhs, RHS& rhs)
			{
				assert(lhs.size() == rhs.size());
				const size_t n = lhs.size();
				resume(n);
				for (size_t i = lhs.size(); i > 0; --i)
				{
					const_type l = lhs[i];
					const_type r = rhs[i];
					const_type p = l * r;
					push_(p);
				}
				return sum();
			}



		private:
			YACK_DISABLE_COPY_AND_ASSIGN(add);
		};
	}
}

#endif


