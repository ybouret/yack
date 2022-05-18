

#ifndef YACK_GFX_FILTERS_INCLUDED
#define YACK_GFX_IMAGE_CODECS_INCLUDED 1

#include "yack/gfx/filter.hpp"
#include "yack/ptr/ark.hpp"
#include "yack/associative/suffix/set.hpp"

namespace yack
{
    namespace graphic
    {

        typedef ark_ptr<string,const filter>   filter_ptr;
        typedef suffix_set<string,filter_ptr>  filter_set;

        //__________________________________________________________________
        //
        //
        //! light-weight shared filters
        //
        //__________________________________________________________________
        class filters
        {
        public:
            static const char Xext[];
            static const char Yext[];
            
            filters(const filter &fx, const filter &fy) throw();
            filters(const filters &)                    throw();
            virtual ~filters() throw();

            const filter_ptr X;
            const filter_ptr Y;
        private:
            YACK_DISABLE_ASSIGN(filters);
        };

    }

}

#endif

