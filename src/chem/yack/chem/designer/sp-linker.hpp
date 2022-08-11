

//! \file

#ifndef YACK_CHEMICAL_DESIGNER_SP_LINKER_INCLUDED
#define YACK_CHEMICAL_DESIGNER_SP_LINKER_INCLUDED 1


#include "yack/jive/syntax/translator.hpp"
#include "yack/hashing/perfect.hpp"

namespace yack
{
    namespace chemical
    {
        namespace nucleus
        {
            //! build sp_info from parsed input
            class sp_linker  : public large_object, public jive::syntax::translator
            {
            public:
                virtual ~sp_linker() throw(); //!< cleanup
                explicit sp_linker();
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(sp_linker);
                const hashing::perfect thash;
                virtual void on_terminal(const lexeme &);
                virtual void on_internal(const string &,const size_t);
                static  void raise_error(const string &);
            };
        }
    }

}

#endif
