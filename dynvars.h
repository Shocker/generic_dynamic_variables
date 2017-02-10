#pragma once

/*
    generic dynamic variables https://github.com/Shocker/generic_dynamic_variables
    by shocker@freakz.ro / iancuta.cornel@gmail.com https://github.com/Shocker
*/

#include <unordered_map>
#include <assert.h>
#include <regex>

#include "any.h"

#define ASSERT(cond, fmt, ...) if (!(cond)) { printf(fmt, ##__VA_ARGS__); *((volatile int*)NULL) = 0; }
class DynVars
{
    public:
        DynVars() : ensureAlreadyExists(false) { }

        typedef std::unordered_map<std::string, anyspace::any> variablesMap;

        template<typename T>
        T * Get(std::string const& key) const
        {
            _ensureVariable(key);
            auto itr = variables.find(key);
            if (itr != variables.end())
                return const_cast<T*>(anyspace::any_cast<T>(&itr->second));
            return nullptr;
        }
        template<typename T>
        T * GetOrCreate(std::string const& key)
        {
            static_assert(std::is_fundamental<T>::value, "GetOrCreate() must be used only for fundamental types, use GetAuto() instead");
            if (!IsSet(key))
                Reset<T>(key);
            return Get<T>(key);
        }
        template<typename T>
        T GetValue(std::string const& key) const
        {
            _ensureVariable(key);
            auto itr = variables.find(key);
            if (itr != variables.end())
                return anyspace::any_cast<T>(itr->second);
            return T();
        }
        DynVars * Set(std::string const& key, anyspace::any&& value)
        {
            _ensureVariable(key);
            variables[key] = std::move(value);
            return this;
        }
        template<typename T>
        DynVars * Set(std::string const& key, T&& value)
        {
            _ensureVariable(key);
            variables[key] = std::move((anyspace::any&&)value);
            return this;
        }
        template<typename T>
        T SetAndReturnValue(std::string const& key, T&& value)
        {
            _ensureVariable(key);
            variables[key] = std::move((anyspace::any&&)value);
            return value;
        }
        anyspace::any SetAndReturnValue(std::string const& key, anyspace::any&& value)
        {
            _ensureVariable(key);
            variables[key] = std::move(value);
            return value;
        }
        template<typename T>
        DynVars * Reset(std::string const& key)
        {
            Set<T>(key, T());
            return this;
        }
        DynVars * ToggleBool(std::string const& key)
        {
            if (IsSet(key))
                Remove(key);
            else
                Set(key, true);
            return this;
        }
        template<typename T>
        T * GetAuto(std::string const& key, bool createIfInexistent = true)
        {
            static_assert(!std::is_fundamental<T>::value, "GetAuto() must not be used for fundamental types, use GetOrCreate() instead");
            _ensureVariable(key);
            if (IsSet(key))
                return *Get<T*>(key);
            else if (!createIfInexistent)
                return nullptr;
            T * value = new T;
            _destructors[key].p = value;
            _destructors[key].destroy = [](const void* x) { static_cast<const T*>(x)->~T(); };
            variables[key] = std::move((anyspace::any&&)value);
            return value;
        }
        DynVars * Remove(std::string const& key, bool regexCaseSensitive = true)
        {
            std::vector<std::string> keys;
            if (key.find("*") != std::string::npos)
            {
                std::string expr = key;
                std::replace(expr.begin(), expr.end(), '.', '|');
                while (expr.find("|") != std::string::npos)
                    expr.replace(expr.find("|"), 1, std::string(R"(\.)"));
                std::replace(expr.begin(), expr.end(), '*', '|');
                while (expr.find("|") != std::string::npos)
                    expr.replace(expr.find("|"), 1, std::string(".*"));
                expr = "^" + expr + "$";
                std::regex regexpr(expr, regexCaseSensitive ? std::regex_constants::syntax_option_type(0) : std::regex_constants::icase);
                for (auto var : variables)
                    if (std::regex_match(var.first, regexpr))
                        keys.push_back(var.first);
            }
            else
                keys.push_back(key);
            for (auto k : keys)
            {
                _destructors.erase(k);
                variables.erase(k);
            }
            return this;
        }
        /*
        // simple remove without regex dependency which may increases file size a lot
        Variables * Remove(std::string const& key)
        {
            _destructors.erase(key);
            variables.erase(key);
            return this;
        }
        */

        bool IsSet(std::string const& key) const { return variables.find(key) != variables.end(); }
        DynVars * Clear() { _destructors.clear(); variables.clear(); return this; }

        void EnsureVariablesAlreadyExist(bool state) { ensureAlreadyExists = state; }

        variablesMap const * GetVariables() const { return &variables; }

    protected:
        variablesMap variables;
        bool ensureAlreadyExists;

    private:
        struct destructor
        {
            const void* p;
            void(*destroy)(const void*);
            ~destructor() { destroy(p); delete (char*)p; }
        };
        std::unordered_map<std::string, destructor> _destructors;
        void _ensureVariable(std::string const& key) const
        {
            if (!ensureAlreadyExists)
                return;
            ASSERT(IsSet(key), "Invalid variable '%s'", key.c_str());
        }
};
