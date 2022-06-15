//! \file

#ifndef YACK_LUAXX_STATE_INCLUDED
#define YACK_LUAXX_STATE_INCLUDED 1

#include "yack/string.hpp"
#include "yack/ptr/arc.hpp"
extern "C"
{
#include "yack/lua/lua.h"
#include "yack/lua/lualib.h"
#include "yack/lua/lauxlib.h"
}

namespace yack
{
    namespace Lua
    {
        //______________________________________________________________________
        //
        //
        //! wrapper for lua_State
        //
        //______________________________________________________________________
        class State : public object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // definition
            //__________________________________________________________________
            static const char dummy_name[]; //!< used for evaluation

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~State() throw(); //!< lua_close
            explicit State();         //!< lua_newstate

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void getglobal(const string &); //!< clear/getglobal
            void getglobal(const char   *); //!< clear/getglobal

            void dofile(const string &);   //!< dofile
            void dofile(const char   *);   //!< dofile
            void dostring(const string &); //!< dostring
            void dostring(const char   *); //!< dostring

            //! conversion to lua_Integer/lua_Number/string
            template <typename T>
            T to(const int idx)
            {
                static const type2type<T> which = {};
                return to(which,idx);
            }

            //! return type at idx
            int type(const int idx) throw();


            //! getglobal and convert
            template <typename T> inline
            T get(const string &name) { getglobal(name); return to<T>(-1); }

            //! getglobal and convert
            template <typename T> inline
            T get(const char   *name) { getglobal(name); return to<T>(-1); }

            //! evaluate expression
            template <typename T> inline
            T eval(const string &expr)
            {
                set_dummy_name(expr);
                return get<T>(dummy_name);
            }

            //! evaluate expression
            template <typename T> inline
            T eval(const char *expr)
            {
                set_dummy_name(expr);
                return get<T>(dummy_name);
            }

            //! access operator
            lua_State * operator*() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(State);
            lua_State *L;
            lua_Integer to(const type2type<lua_Integer> &, const int);
            lua_Number  to(const type2type<lua_Number>  &, const int);
            string      to(const type2type<string>      &, const int);
            void        set_dummy_name(const string &);
            void        set_dummy_name(const char   *);
        };

        //______________________________________________________________________
        //
        //
        //! shared lua state
        //
        //______________________________________________________________________
        typedef arc_ptr<State> VM;
    }
}

#endif

