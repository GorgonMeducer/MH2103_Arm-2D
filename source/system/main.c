#include "inc.h"
#include "mh.h"
#include "perf_counter.h"
#include "arm_2d_helper.h"
#include "arm_2d_scenes.h"
#include "arm_2d_disp_adapters.h"
#include "arm_2d_disp_adapter_0.h"
#include "arm_2d_scene_watch_face_01.h"
#include "arm_2d_example_controls.h"
#include "arm_2d_demos.h"

#include "arm_2d_scene_knob.h"
#include "arm_2d_scene_fan.h"
#include "arm_2d_scene_watch.h"
#include "arm_2d_scene_alarm_clock.h"
#include "arm_2d_scene_fitness.h"
#include "arm_2d_scene_histogram.h"
#include "arm_2d_scene_meter.h"
#include "arm_2d_scene_jntm.h"

#if defined(RTE_Acceleration_Arm_2D_Extra_Benchmark)
#   include "arm_2d_benchmark.h"
#endif

static
void SysTest(void);

int main(void)
{
    init_cycle_counter(true); //don't init systick

    arm_irq_safe {
        arm_2d_init();
    }
    disp_adapter0_init();
#if defined(RTE_Acceleration_Arm_2D_Extra_Benchmark)
    arm_2d_run_benchmark();
#else
    /*******************************************************************************/
    {//show demo
        //arm_2d_scene_filters_init(&DISP0_ADAPTER);
        //arm_2d_scene_text_reader_init(&DISP0_ADAPTER);
        //arm_2d_scene_fan_init(&DISP0_ADAPTER);
        //arm_2d_scene_knob_init(&DISP0_ADAPTER);
        //arm_2d_scene_compass_init(&DISP0_ADAPTER);
        //arm_2d_scene_gas_gauge_init(&DISP0_ADAPTER);
        //arm_2d_scene_watch_init(&DISP0_ADAPTER);
        //arm_2d_scene_alarm_clock_init(&DISP0_ADAPTER);
        //arm_2d_scene_fitness_init(&DISP0_ADAPTER);
        //arm_2d_scene_tjpgd_init(&DISP0_ADAPTER);
        //arm_2d_scene_histogram_init(&DISP0_ADAPTER);
        //arm_2d_scene_meter_init(&DISP0_ADAPTER);
        //arm_2d_scene_user_defined_opcode_init(&DISP0_ADAPTER);
        //arm_2d_scene_jntm_init(&DISP0_ADAPTER);
        //arm_2d_scene_watch_face_01_init(&DISP0_ADAPTER);
        arm_2d_scene_matrix_init(&DISP0_ADAPTER);
        //arm_2d_scene_progress_status_init(&DISP0_ADAPTER);
    }
    arm_2d_scene_player_switch_to_next_scene(&DISP0_ADAPTER);
    /*******************************************************************************/
#endif

#if defined(__PERF_COUNTER_COREMARK__)
    coremark_main();
#endif

    while (1) {
        while(arm_fsm_rt_cpl !=disp_adapter0_task());
    }
}


//CCT6: LQFP48,flash = 256KB,ram=64K
//RCT6: LQFP64,flash = 256KB,ram=64K
//AVGT6: LQFP100,flash = 1024KB,ram=96K
static
void SysTest(void)
{
    volatile uint32_t t,id1,id2,id3;
    volatile uint32_t sram,flash;

    t = *((uint32_t*)0x1FFFF7E0);
    printf("Flash size =0x%04X\r\n",t & 0xFFFF);
    //0x0080：FLASH 容量是 128K 字节。

//    printf("chip RAM=0x%02X,FLASH=0x%02X\r\n",sram>>4,flash);
//    id1 = *((uint32_t*)0x1FFFF7E0+0x08); //chip id,96bit
//    id2 = *((uint32_t*)0x1FFFF7E0+0x0C); //chip id,96bit
//    id2 = *((uint32_t*)0x1FFFF7E0+0x10); //chip id,96bit
//    printf("chip tag 96bits ID1=0x%08X,ID2=0x%08X,ID3=0x%08X\r\n",id1,id2,id3);
}
/*
Boot_JumpToApp（msp,jumpaddr);
Boot_JumpToApp  PROC
                 EXPORT  Boot_JumpToApp             [WEAK]
                 CPSID   I             ; Mask interrupts
                 MSR     MSP, R0  ; 设置主栈指针 (MSP) 
                 BX        R1           ; 跳转到复位处理函数
                 ENDP
*/