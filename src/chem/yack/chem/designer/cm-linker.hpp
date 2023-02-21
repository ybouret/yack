
//! \file

#ifndef YACK_CHEMICAL_DESIGNER_CM_LINKER_INCLUDED
#define YACK_CHEMICAL_DESIGNER_CM_LINKER_INCLUDED 1


#include "yack/jive/syntax/translator.hpp"
#include "yack/hashing/perfect.hpp"

namespace yack
{
    namespace chemical
    {
        namespace nucleus
        {
            //__________________________________________________________________
            //
            //
            //! linker for components
            //
            //__________________________________________________________________
            class cm_linker  : public large_object, public jive::syntax::translator
            {
            public:

                virtual ~cm_linker() noexcept; //!< cleanup
                explicit cm_linker();         //!< setup


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(cm_linker);
                list<int>              ilist;
                string                 sname;
                const hashing::perfect thash;
                const hashing::perfect ihash;

                virtual void on_init();         //!< reset ilist
                
                virtual void on_terminal(const lexeme &);
                virtual void on_internal(const string &,const size_t);
                static  void raise_error(const string &);
                void         reduce() noexcept;
            };
        }
    }

}

#endif
