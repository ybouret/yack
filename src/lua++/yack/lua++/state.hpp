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
            virtual ~State() throw(); //!< lua_close
            explicit State();         //!< lua_newstate

            void getglobal(const string &);
            void getglobal(const char   *);

            void dofile(const string &);
            void dofile(const char   *);
            void dostring(const string &);
            void dostring(const char   *);

            template <typename T>
            T to(const size_t idx)
            {
                static const type2type<T> which = {};
                return to(which,idx);
            }

            template <typename T> inline
            T get(const string &name) { getglobal(name); return to<T>(-1); }

            template <typename T> inline
            T get(const char   *name) { getglobal(name); return to<T>(-1); }

            lua_State * operator*() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(State);
            lua_State *L;
            lua_Integer to(const type2type<lua_Integer> &, const int);
            lua_Number  to(const type2type<lua_Number>  &, const int);
            string      to(const type2type<string>      &, const int);
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

