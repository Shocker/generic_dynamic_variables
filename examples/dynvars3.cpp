#include "../dynvars.h"
#include <iostream>

int main()
{
    DynVars vars;

    std::string varName;
    std::cout << "Enter variable name to set: ";
    std::cin >> varName;

    int varType = 0;
    do
    {
        std::cout << "Enter variable type (1: int, 2: float): ";
        std::cin >> varType;
    } while (varType < 1 || varType > 2);

    std::cout << "Enter variable value: ";
    switch (varType)
    {
        case 1:
        {
            int x;
            scanf("%d", &x);
            vars.Set(varName, x);
            break;
        }
        case 2:
        {
            float x;
            scanf("%f", &x);
            vars.Set(varName, x);
            break;
        }
    }

    do 
    {
        std::cout << "Enter variable name you would like to check: ";
        std::cin >> varName;
    } while (!vars.IsSet(varName) && [varName]() { std::cout << "Variable '" << varName << "' is not set, try again" << std::endl; return true; }());

    do
    {
        try
        {
            std::cout << "Enter type of variable '" << varName << "' (1: int, 2: float): ";
            std::cin >> varType;

            if (varType == 1)
                std::cout << varName << " = " << *vars.Get<int>(varName);
            else if (varType == 2)
                std::cout << varName << " = " << *vars.Get<float>(varName);
            else
                continue;
            std::cout << std::endl;
            break;
        }
        catch (anyspace::bad_any_cast)
        {
            std::cout << "Variable type entered does not match the defined type, try again" << std::endl;
        }
    } while (true);

    #if defined(WIN32)
    getchar();
    getchar();
    #endif

    return 0;
}
