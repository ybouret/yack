//! \file

#ifndef YACK_JSON_COMPILER_INCLUDED
#define YACK_JSON_COMPILER_INCLUDED 1

#include "yack/json/value.hpp"
#include "yack/singleton.hpp"
#include "yack/jive/module.hpp"

namespace yack
{


    namespace JSON
    {
        
        class Compiler : public singleton<Compiler>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const at_exit::longevity life_time = 13; //!< longevity
            static const char               call_sign[];    //!< "JSON"
            class Reader;
            class Linker;

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            Value & operator()(jive::module *);
            void gc() noexcept;
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Compiler);
            friend class singleton<Compiler>;


            Reader *reader;
            Linker *linker;

            explicit Compiler();
            virtual ~Compiler() noexcept;
            void     cleanup() noexcept;

        };
    }

}

#endif

