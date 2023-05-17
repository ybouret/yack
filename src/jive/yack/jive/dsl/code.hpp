//! \file

#ifndef YACK_JIVE_DSL_CODE_INCLUDED
#define YACK_JIVE_DSL_CODE_INCLUDED 1


#include "yack/hashing/perfect.hpp"
#include "yack/ptr/counted-interface.hpp"
#include "yack/ptr/arc.hpp"

namespace yack
{
    namespace jive
    {

        //----------------------------------------------------------------
        //
        // defines for Terminals
        //
        //----------------------------------------------------------------
#define YACK_DSL_x2B              0 //!< "+"
#define YACK_DSL_x2A              1 //!< "*"
#define YACK_DSL_x3F              2 //!< "?"
#define YACK_DSL_JSTRING          3 //!< "JSTRING"
#define YACK_DSL_RSTRING          4 //!< "RSTRING"
#define YACK_DSL_BSTRING          5 //!< "BSTRING"
#define YACK_DSL_MODULE_ID        6 //!< "MODULE_ID"
#define YACK_DSL_RID              7 //!< "RID"
#define YACK_DSL_PID              8 //!< "PID"
#define YACK_DSL_LID              9 //!< "LID"

        //----------------------------------------------------------------
        //
        // defines for Internals
        //
        //----------------------------------------------------------------
#define YACK_DSL_MODULE        0 //!< "MODULE"
#define YACK_DSL_ALT           1 //!< "ALT"
#define YACK_DSL_AGG           2 //!< "AGG"
#define YACK_DSL_JRK           3 //!< "JRK"
#define YACK_DSL_RULE          4 //!< "RULE"
#define YACK_DSL_PLG           5 //!< "PLG"
#define YACK_DSL_LEX           6 //!< "LEX"
#define YACK_DSL_INC           7 //!< "INC"


        //----------------------------------------------------------------------
        //
        //! hash code from dsl_parser
        //
        //----------------------------------------------------------------------
        class dsl_code : public object
        {
        public:
            //------------------------------------------------------------------
            //
            // definitions
            //
            //------------------------------------------------------------------
            typedef arc_ptr<const dsl_code> handle; //!< alias

            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            explicit dsl_code();          //!< setup tables
            virtual ~dsl_code() noexcept; //!< cleanup

            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            YACK_COUNTED_DECL(); //!< counted interface


            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const hashing::perfect terminal_hash; //!< hash for terminals
            const hashing::perfect internal_hash; //!< hash for internals


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(dsl_code);
            size_t nref;
        };
    }

}

#endif
