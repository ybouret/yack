
//! \file

#ifndef YACK_LUAXX_FUNCTION_INCLUDED
#define YACK_LUAXX_FUNCTION_INCLUDED 1

#include "yack/lua++/state.hpp"

namespace yack
{
    namespace Lua
    {
        //______________________________________________________________________
        //
        //
        //! base class for functions
        //
        //______________________________________________________________________
        class Function_ : public VM
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Function_()  noexcept;          //!< cleanup

        protected:
            explicit Function_(const Function_ &);  //!< copy
            template <typename NAME> inline
            explicit Function_(const VM &vm, const NAME &id) :
            VM(vm), name(id) {}                     //!< setup

            //__________________________________________________________________
            //
            // method
            //__________________________________________________________________

            //! settop(0) and getglobal(name)
            void fetch();

            //! push a value, cast to lua_Number
            template <typename T> inline
            void push(const T &value)
            {
                lua_pushnumber(***this,static_cast<lua_Number>(value));
            }

            //! return the top value, cast from lua_Number
            template <typename T> inline
            T ret()
            {
                lua_State *L = ***this;
                const T    f = static_cast<T>(lua_tonumber(L,-1));
                lua_pop(L,1);
                return f;
            }

            //! protected call with nargs
            void call(int nargs);


        private:
            YACK_DISABLE_ASSIGN(Function_);

        public:
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name; //!< function name
        };


        //______________________________________________________________________
        //
        //
        //! functionoid
        //
        //______________________________________________________________________
        template <typename T>
        class Function : public Function_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            template <typename NAME>
            inline explicit Function(const VM &vm, const NAME &id) : Function_(vm,id) {} //!< setu
            inline          Function(const Function &F) : Function_(F) {}                //!< copy
            inline virtual ~Function() noexcept {}                                        //!< cleanup

            //! call operator, nargs=1
            inline T operator()(const T x)
            {
                fetch();
                push(x);
                call(1);
                return ret<T>();
            }

            //! call operator, nargs=2
            inline T operator()(const T x, const T y)
            {
                fetch();
                push(x);
                push(y);
                call(2);
                return ret<T>();
            }




        private:
            YACK_DISABLE_ASSIGN(Function);
        };


    }

}

#endif
