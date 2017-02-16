#include "../dynvars.h"

class Config : public DynVarsSafe
{
    public:
        int GetInt(std::string const& key) { return GetValue<int>(key); }
        bool GetBool(std::string const& key) { return GetValue<bool>(key); }
        std::string GetString(std::string const& key) { return GetValue<std::string>(key); }
        const char* GetCString(std::string const& key) { return (*Get<std::string>(key)).c_str(); }

        void SetInt(std::string const& key, int value) { Set(key, int(value)); }
        void SetBool(std::string const& key, bool value) { Set(key, bool(value)); }
        void SetString(std::string const& key, std::string value) { Set(key, std::string(value)); }
};

int main()
{
    Config config;

    // add variables
    config.SetString("ZooName", "HappyAnimals");
    config.SetBool("Allow.Cats", true);
    config.SetBool("Allow.Dogs", false);
    config.SetInt("Limits.Animals", 5);

    // don't allow adding new variables anymore or reading inexistent variables
    config.EnsureVariablesAlreadyExist(true);
    //config.SetBool("inexistentVariable", true); // this will cause a crash because the variable doesns't exist, we can only change existent ones
    //config.GetInt("inexistentVariable"); // this will cause a crash because the variable doesn't exist

    printf("Welcome to %s zoo !\n", config.GetCString("ZooName"));
    printf(" - We %sallow Cats\n", config.GetBool("Allow.Cats") ? "" : "don't ");
    printf(" - We %sallow Dogs\n", config.GetBool("Allow.Dogs") ? "" : "don't ");
    printf(" - We can store at most %d animals in here\n", config.GetInt("Limits.Animals"));

    #if defined(WIN32)
    getchar();
    #endif

    return 0;
}
