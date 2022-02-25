
//! \file

#ifndef YACK_GFX_ARTICLE_INCLUDED
#define YACK_GFX_ARTICLE_INCLUDED 1

#include "yack/object.hpp"
#include "yack/counted.hpp"


namespace yack
{
    namespace graphic
    {

        class article : public object, public counted
        {
        public:
            virtual ~article() throw();
        protected:
            explicit article() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(article);
        };
    }

}

#endif
