
//! \file

#ifndef YACK_DATA_SLIM_SOLO_LIST_INCLUDED
#define YACK_DATA_SLIM_SOLO_LIST_INCLUDED 1

#include "yack/data/slim/solo.hpp"
#include "yack/data/slim/registry.hpp"

namespace yack
{

	//__________________________________________________________________________
	//
	//
	//! list of objects with internal cache
	//
	//__________________________________________________________________________
	template <typename T>
	class solo_list : public slim_solo<T, slim_node<T>, yack::slim_registry>
	{
	public:
		//______________________________________________________________________
		//
		// types
		//______________________________________________________________________
		typedef slim_node<T>                               node_type; //!< alias
		typedef slim_solo<T,node_type,yack::slim_registry> body_type; //!< alias,

		//______________________________________________________________________
		//
		// C++
		//______________________________________________________________________
		inline explicit solo_list() throw()  : body_type()  {} //!< setup
		inline virtual ~solo_list() throw()                 {} //!< cleanup
		inline solo_list(const solo_list &_) : body_type(_) {} //!< delegate copy

	private:
		YACK_DISABLE_ASSIGN(solo_list);
	};

}

#endif
