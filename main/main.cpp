#include "test.h"
#include <iostream> // C++标准头文件，其中包含cout和endl的定义。


using namespace std;

void Handler(int signo)
{
    // System Exit
    std::cout << "Handler: exit" << std::endl;
    DEV_ModuleExit();

    exit(0);
}

int main()
{
    signal(SIGINT, Handler);
    OLED_show();

    return 0;
}
