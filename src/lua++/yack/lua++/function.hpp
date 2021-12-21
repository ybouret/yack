
//! \file

#ifndef YACK_LUAXX_FUNCTION_INCLUDED
#define YACK_LUAXX_FUNCTION_INCLUDED 1

#include "yack/lua++/state.hpp"

namespace yack
{
    namespace Lua
    {

        class Function_ : public VM
        {
        public:
            const string name;

            virtual ~Function_() throw();

        protected:
            template <typename NAME> inline
            explicit Function_(const VM &vm, const NAME &id) :
            VM(vm), name(id) {}

            Function_(const Function_ &);
            void fetch();
            template <typename T> inline
            void push(const T &value)
            {
                lua_pushnumber(***this,static_cast<lua_Number>(value));
            }


        private:
            YACK_DISABLE_ASSIGN(Function_);
        };

        template <typename T>
        class Function : public Function_
        {
        public:

            template <typename NAME>
            inline explicit Function(const VM &vm, const NAME &id) : Function_(vm,id) {}
            inline          Function(const Function &F) : Function_(F) {}
            inline virtual ~Function() throw() {}

            inline T operator()(const T x)
            {
                fetch();
                push(x);
            }



        private:
            YACK_DISABLE_ASSIGN(Function);
        };


    }

}

#endif
