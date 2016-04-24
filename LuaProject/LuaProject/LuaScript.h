#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "lua.hpp"

class LuaScript
{
public:
	LuaScript(const std::string& filename);
	~LuaScript();
	void printError(const std::string& variableName, const std::string reason);
	std::vector<int> getIntVector(const std::string& variableName);

	void clean() {
		int n = lua_gettop(L);
		lua_pop(L, n);
	}

	template <typename T>
	T get(const std::string& variableName) {
		if (!L)
		{
			printError(variableName, "Script is not loaded");
			return lua_getdefault();
		}

		T result;
		if (lua_gettostack(variableName))
		{
			result = lua_get(variableName);
		}
		else
		{
			result = lua_getdefault();
		}
		lua_pop(L, level + 1);
		return result;
	}
	bool lua_gettostack(const std::string& variableName) {
		level = 0;
		std::string var = "";
		for (unsigned int i = 0; i < variableName.size(); i++)
		{
			if (variableName.at(i) == '.')
			{
				if (level == 0)
				{
					lua_getglobal(L, var.c_str());
				}
				else
				{
					lua_getfield(L, -1, var.c_str());
				}
				if (lua_isnil(L, -1))
				{
					printError(variableName, var + " is not defined");
				}
				else
				{
					var = "";
					level++;
				}
			}
			else
			{
				var += variableName.at(i);
			}
		}
		if (level == 0)
		{
			lua_getglobal(L, var.c_str());
		}
		else
		{
			lua_getfield(L, -1, var.c_str());
		}
		if (lua_isnil(L, -1))
		{
			printError(variableName, var + " is not defined");
			return false;
		}
		return true;
	}

	template <typename T>
	T lua_get(const std::string& variableName) {
		return 0;
	}
	template <>
	inline bool LuaScript::lua_get(const std::string& variableName) {
		return (bool)lua_toboolean(L, -1);
	}
	template<>
	inline float LuaScript::lua_get(const std::string& variableName){
		if (!lua_isnumber(L, -1))
		{
			printError(variableName, "Not a numbler");
		}
		return (float)lua_tonumber(L, -1);
	}
	template<>
	inline int LuaScript::lua_get(const std::string& variableName) {
		if (!lua_isnumber(L, -1))
		{
			printError(variableName, "Not a numbler");
		}
		return (int)lua_tonumber(L, -1);
	}
	template<>
	inline std::string LuaScript::lua_get(const std::string& variableName) {
		std::string s = "null";
		if (lua_isstring(L, -1))
		{
			s = std::string(lua_tostring(L, -1));
		}
		else
		{
			printError(variableName, "Not a string");
		}
		return s;
	}

	template <typename T>
	T lua_getdefault(const std::string& variableName) {
		return 0;
	}
	template <>
	inline std::string LuaScript::lua_getdefault(const std::string& variableName) {
		return "null";
	}

private:
	lua_State* L;
	int level = 0;
};

