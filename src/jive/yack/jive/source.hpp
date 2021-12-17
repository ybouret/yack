
//! \file

#ifndef YACK_JIVE_SOURCE_INCLUDED
#define YACK_JIVE_SOURCE_INCLUDED 1

#include "yack/jive/module.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    namespace jive
    {
        //______________________________________________________________________
        //
        //
        //! transform module into a stream of jive::character
        //
        //______________________________________________________________________
        class source
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit source(module *) throw(); //!< setup
            virtual ~source()         throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            character       *query();                     //!< query next char, NULL => EOF
            void             store(character *) throw();  //!< store read char
            void             store(token &)     throw();  //!< store read token
            void             store_copy(const token &);   //!< store a copy of a read token
            const character *peek();                      //!< peek next char, NULL => EOF
            void             skip() throw();              //!< skip first cached characters
            void             skip(size_t n) throw();      //!< skip first n cached characters
            bool             done();                      //!< NULL == peek()
            size_t           read()      const throw();   //!< cache size
            module         & operator*()       throw();   //!< *intake
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(source);
            auto_ptr<module> intake;
            characters       cache;
        };

    }

}

#endif

