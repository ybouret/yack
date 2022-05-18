

#ifndef YACK_GFX_FILTERS_INCLUDED
#define YACK_GFX_IMAGE_CODECS_INCLUDED 1

#include "yack/gfx/filter.hpp"
#include "yack/ptr/ark.hpp"
#include "yack/associative/suffix/set.hpp"

namespace yack
{
    namespace graphic
    {

        typedef ark_ptr<string,const filter>   filter_ptr; //!< alias
        typedef suffix_set<string,filter_ptr>  filter_set; //!< alias

        //__________________________________________________________________
        //
        //
        //! light-weight shared filters
        //
        //__________________________________________________________________
        class filters
        {
        public:
            static const char Xext[]; //!< "X"
            static const char Yext[]; //!< "Y"


            filters(const filter &fx, const filter &fy) throw(); //!< setup from ALLOCATED filters
            filters(const filters &)                    throw(); //!< no-throw copy
            virtual ~filters()                          throw(); //!< cleanup

            const filter_ptr X; //!< X-Filter
            const filter_ptr Y; //!< Y-Filter
            
        private:
            YACK_DISABLE_ASSIGN(filters);
        };

    }

}

#endif

