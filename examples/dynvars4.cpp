#include <vector>
#include <thread>

#include "../dynvars.h"

class someClass
{
    public:
        someClass() { printf(" - someClass created\n"); }
        ~someClass() { printf(" - someClass destroyed, value = %d, secondVal = %d\n", val, secondVal); }
        int val = 0;
        int secondVal = 0;
};

DynVarsSafe vars;

void testThreads()
{
    // fundamental types
    vars.Set<int>("anInt", 14); // manually define type
    printf("anInt = %d\n\n", vars.GetValue<int>("anInt"));

    char aChar = 'x';
    vars.Set("aChar", aChar); // automatically deduce type
    printf("aChar = %c\n\n", vars.GetValue<char>("aChar"));

    float * aFloat = vars.GetOrCreate<float>("aFloat");
    printf("aFloat = %f\n", *aFloat);
    *aFloat = 3.14f;
    printf("aFloat = %f\n\n", *aFloat);

    int unsetVariable = vars.GetValue<int>("unsetVariable");
    printf("Is 'unsetVariable' set = %d, unsetVariable value = %d\n\n", vars.IsSet("unsetVariable"), unsetVariable);

    printf("anotherInt = %d\n\n", vars.SetAndReturnValue<int>("anotherInt", 15));

    // set 4 variables and remove one in the same line
    vars.Set<int>("int2", 22)->Set<int>("int3", 33)->Set<int>("int4", 44)->Remove("int4");
    printf("int2 = %d, &int3 = %p, int3 = %d, &int4 = %p, int4 = %d\n\n", vars.GetValue<int>("int2"), vars.Get<int>("int3"), vars.GetValue<int>("int3"), vars.Get<int>("int4"), vars.GetValue<int>("int4"));

    // remove variables by wildcard name
    vars.Lock();
    DynVars::variablesMap const * list = vars.GetVariables();
    printf("current variables: ");
    for (auto varData : *list)
        printf("%s, ", varData.first.c_str());
    printf("\nremoving *int* named vars, remaining: ");
    vars.Remove("*int*", false);
    for (auto varData : *list)
        printf("%s, ", varData.first.c_str());
    printf("\n\n");
    vars.Unlock();

    // create another variable for a non-fundamental std type
    vars.Lock();
    auto intVector = vars.GetAuto<std::vector<int>>("intVector");
    intVector->push_back(123);
    intVector->push_back(456);
    printf("vector has size = %d, v[0] = %d, v[1] = %d\n\n", int(intVector->size()), (*intVector)[0], (*intVector)[1]);
    vars.Unlock();

    // create a variable for a non-fundamental custom class
    printf("someClass should be created next:\n");
    vars.Lock();
    vars.GetAuto<someClass>("someClassInstance")->val = 16; // create a someClass instance and assign field "val" = 16;
    auto someClassInstance = vars.GetAuto<someClass>("someClassInstance"); // get the previously created instance of someClass
    someClassInstance->secondVal = 34; // and assign field "secondVal" = 34;
    printf("someClass should be destroyed next:\n");
    vars.Remove("someClassInstance"); // remove the someClassInstance variable (which will automatically free up memory and call destructor)
    vars.Unlock();
    printf("\n");
}

int main()
{
    std::vector<std::thread> threads;

    do
    {
        threads.clear();
        try
        {
            for (int i = 0; i < 50; i++)
                threads.push_back(std::thread([]() { testThreads(); }));
            for (auto& thread : threads)
                thread.join();
        }
        catch (const std::system_error& e)
        {
            printf("Caught system_error with code %d meaning %s\n", e.code().value(), e.what());
        }
    } while (true);

    return 0;
}
