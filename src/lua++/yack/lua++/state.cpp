
#include "yack/lua++/state.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace Lua
    {
        const char State:: dummy_name[] = "dummy_name";

        State:: ~State() throw()
        {
            lua_close(L);
        }

        State:: State() :
        L( luaL_newstate() )
        {
            if(!L) throw exception("luaL_newstate failure");
            luaL_openlibs(L);
        }

        lua_State * State:: operator*() throw()
        {
            assert(L);
            return L;
        }


        void State:: getglobal(const string &name)
        {
            lua_settop(L,0);
            lua_getglobal(L,name());
            
        }

        void State:: getglobal(const char   *name)
        {
            const string _(name);
            return getglobal(_);
        }

        void State:: dofile(const string &filename)
        {
            if(LUA_OK!=luaL_dofile(L,filename()))
            {
                throw exception("luaL_dofile(%s): %s",filename(),lua_tostring(L,-1));
            }
        }

        void  State:: dostring(const string &buffer)
        {
            if(LUA_OK!=luaL_dostring(L,buffer()))
            {
                throw exception("luaL_dostring: %s",lua_tostring(L,-1));
            }
        }


        void  State:: dostring(const char *buffer)
        {
            const string _(buffer);
            dostring(_);
        }


        void State:: dofile(const char   *filename)
        {
            const string _(filename);
            dofile(_);
        }


        lua_Integer State:: to(const type2type<lua_Integer> &, const int idx)
        {
            if(!lua_isinteger(L,idx))
            {
                throw exception("lua_isinteger got %s",luaL_typename(L,idx));
            }
            return lua_tointeger(L,idx);
        }

        lua_Number State:: to(const type2type<lua_Number> &, const int idx)
        {
            if(!lua_isnumber(L,idx))
            {
                throw exception("lua_isnumber got %s",luaL_typename(L,idx));
            }
            return lua_tonumber(L,idx);
        }

        string State::to(const type2type<string> &, const int idx)
        {
            if(!lua_isstring(L,idx))
            {
                throw exception("lua_isstring got %s",luaL_typename(L,idx));
            }
            size_t      len = 0;
            const char *str = lua_tolstring(L,idx,&len);
            return string(str,len);
        }

        void State:: set_dummy_name(const string &expr)
        {
            const string code = dummy_name + ( '=' + expr);
            dostring(code);
        }

        void State:: set_dummy_name(const char *expr)
        {
            const string _(expr);
            set_dummy_name(_);
        }


        int State:: type(const int idx) throw()
        {
            return lua_type(L,idx);
        }
    }
}
