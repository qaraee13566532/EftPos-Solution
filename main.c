#include "mcc_generated_files/system.h"
#include "variables.h"
#include "HardwareProfile.h"
#include <math.h>
#include <xc.h>

extern void Pc_Command_Services(void);
extern void InitMainPrograms(void);


void Global_Services(void)
{
    Pc_Command_Services();
}


int main(void)
{
    SYSTEM_Initialize();
    beez_time=BEEP_50ms;
    InitMainPrograms();

    for(;;)
    {   
        Global_Services();
    }
}

