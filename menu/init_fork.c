#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "menu.h"
#include "proc.h"

int main()
{
    PrintMenuOS();
    SetPrompt("MenuOS>>");
    MenuConfig("version","MenuOS V1.0(Based on Linux 3.18.6)",NULL);
    MenuConfig("quit","Quit from MenuOS",Quit);
    MenuConfig("time","Show System Time",Time);
    MenuConfig("time-asm","Show System Time(asm)",TimeAsm);
    MenuConfig("fork","Fork a new process",Fork);
    ExecuteMenu();
}

