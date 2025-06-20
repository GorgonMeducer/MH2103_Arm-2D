/*
 * Copyright (c) 2009-2024 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __ARM_2D_DISP_ADAPTER0_H__
#define __ARM_2D_DISP_ADAPTER0_H__

#include "arm_2d_helper.h"

#ifdef RTE_Acceleration_Arm_2D_Helper_Disp_Adapter0

#ifdef   __cplusplus
extern "C" {
#endif

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wunknown-warning-option"
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#   pragma clang diagnostic ignored "-Wdeclaration-after-statement"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------

// <h>Screen and Framebuffer
// =======================

// <o> Select the screen colour solution
//     <0=>     None
//     <1=>     Monochrome
// <i> When specifying a colour solution, the __DISP0_CFG_COLOUR_DEPTH__ and other corresponding options will be configured (overriden) accordingly.
#ifndef __DISP0_CFG_COLOR_SOLUTION__
#   define __DISP0_CFG_COLOR_SOLUTION__                            0
#endif

// <o> Select the screen colour depth
//     <8=>     8 Bits
//     <16=>    16Bits
//     <32=>    32Bits
// <i> The colour depth of your screen
#ifndef __DISP0_CFG_COLOUR_DEPTH__
#   define __DISP0_CFG_COLOUR_DEPTH__                              16
#endif

// <o>Width of the screen <8-32767>
// <i> The width of your screen
// <i> Default: 320
#ifndef __DISP0_CFG_SCEEN_WIDTH__
#   define __DISP0_CFG_SCEEN_WIDTH__                               320
#endif

// <o>Height of the screen <8-32767>
// <i> The height of your screen
// <i> Default: 240
#ifndef __DISP0_CFG_SCEEN_HEIGHT__
#   define __DISP0_CFG_SCEEN_HEIGHT__                              240
#endif

/*
  ARM_SCREEN_NO_ROTATION   0
  ARM_SCREEN_ROTATE_90     1
  ARM_SCREEN_ROTATE_180    2
  ARM_SCREEN_ROTATE_270    3
 */

// <o>Rotate the Screen
//     <0=>  NO Rotation
//     <1=>    90 Degree
//     <2=>   180 Degree
//     <3=>   270 Degree
// <i> Rotate the Screen for specified degrees.
// <i> NOTE: This is extremely slow. Please avoid using it whenever it is possible.
#ifndef __DISP0_CFG_ROTATE_SCREEN__
#   define __DISP0_CFG_ROTATE_SCREEN__                             0
#endif

// <o>Width of the PFB block
// <i> The width of your PFB block size used in disp0
#ifndef __DISP0_CFG_PFB_BLOCK_WIDTH__
#   define __DISP0_CFG_PFB_BLOCK_WIDTH__                           320
#endif

// <o>Height of the PFB block
// <i> The height of your PFB block size used in disp0
#ifndef __DISP0_CFG_PFB_BLOCK_HEIGHT__
#   define __DISP0_CFG_PFB_BLOCK_HEIGHT__                          60
#endif

// <o>Width Alignment of generated PFBs
//     <0=>     1 pixel
//     <1=>     2 pixel
//     <2=>     4 pixel
//     <3=>     8 pixel
//     <4=>    16 pixel
//     <5=>    32 pixel
//     <6=>    64 pixel
//     <7=>   128 pixel
// <i> Make sure the x and width of the PFB is always aligned to 2^n pixels
#ifndef __DISP0_CFG_PFB_PIXEL_ALIGN_WIDTH__
#   define __DISP0_CFG_PFB_PIXEL_ALIGN_WIDTH__                     1
#endif

// <o>Height Alignment of generated PFBs
//     <0=>     1 pixel
//     <1=>     2 pixel
//     <2=>     4 pixel
//     <3=>     8 pixel
//     <4=>    16 pixel
//     <5=>    32 pixel
//     <6=>    64 pixel
//     <7=>   128 pixel
// <i> Make sure the y and height of the PFB is always aligned to 2^n pixels
#ifndef __DISP0_CFG_PFB_PIXEL_ALIGN_HEIGHT__
#   define __DISP0_CFG_PFB_PIXEL_ALIGN_HEIGHT__                    0
#endif

// <o>PFB Block Count <1-65535>
// <i> The number of blocks in the PFB pool.
#ifndef __DISP0_CFG_PFB_HEAP_SIZE__
#   define __DISP0_CFG_PFB_HEAP_SIZE__                             1
#endif

// </h>

// <h>Navigation Layer
// =======================

// <o>Navigation Layer Mode
//     <0=>     Disable Navigation Layer
//     <1=>     Normal Mode (Bottom)
//     <2=>     Tiny Mode (Bottom Centre)
// <i> Configure the default navigation layer of this display adapter. 
// <i> NOTE: Disable the navigation layer will also remove the real-time FPS display.
#ifndef __DISP0_CFG_NAVIGATION_LAYER_MODE__
#   define __DISP0_CFG_NAVIGATION_LAYER_MODE__                     0
#endif

// <o>Number of iterations <0-2000>
// <i> run number of iterations before calculate the FPS.
#ifndef __DISP0_CFG_ITERATION_CNT__
#   define __DISP0_CFG_ITERATION_CNT__                             30
#endif

// <o>FPS Calculation Mode
//     <0=>     Render-Only FPS
//     <1=>     Real FPS
// <i> Decide the meaning of the real time FPS display
#ifndef __DISP0_CFG_FPS_CACULATION_MODE__
#   define __DISP0_CFG_FPS_CACULATION_MODE__                       1
#endif

// <q> Enable Console
// <i> Add a simple console to the display adapter in a floating window.
// <i> This feature is disabled by default.
#ifndef __DISP0_CFG_USE_CONSOLE__
#   define __DISP0_CFG_USE_CONSOLE__                                0
#endif

// <o> Console Input Buffer Size
// <i> The size of console input buffer, 0 means no input buffer
#ifndef __DISP0_CFG_CONSOLE_INPUT_BUFFER__
#   define __DISP0_CFG_CONSOLE_INPUT_BUFFER__                       255
#endif

// <o> Console Display Time in ms <1000-0xFFFFFFFF>
// <i> The time before the console disappear for each content update.
#ifndef __DISP0_CFG_CONSOLE_DISPALY_TIME__
#   define __DISP0_CFG_CONSOLE_DISPALY_TIME__                       3000
#endif

// </h>

// <h>Optimization and Misc
// =======================

// <q> Enable Dirty Region Debug Mode
// <i> Draw dirty regions on the screen for debug.
#ifndef __DISP0_CFG_DEBUG_DIRTY_REGIONS__
#   define __DISP0_CFG_DEBUG_DIRTY_REGIONS__                       0
#endif

// <q> Enable Dirty Region Optimization Service
// <i> Optimize dirty regions to avoid fresh overlapped areas
#ifndef __DISP0_CFG_OPTIMIZE_DIRTY_REGIONS__
#   define __DISP0_CFG_OPTIMIZE_DIRTY_REGIONS__                    1
#endif

// <o> Dirty Region Pool Size <4-255>
// <i> The number of dirty region items available for the dirty region optimization service
#ifndef __DISP0_CFG_DIRTY_REGION_POOL_SIZE__
#   define __DISP0_CFG_DIRTY_REGION_POOL_SIZE__                    8
#endif

// <q> Swap the high and low bytes
// <i> Swap the high and low bytes of the 16bit-pixels
#ifndef __DISP0_CFG_SWAP_RGB16_HIGH_AND_LOW_BYTES__
#   define __DISP0_CFG_SWAP_RGB16_HIGH_AND_LOW_BYTES__             0
#endif

// <q>Enable the helper service for Asynchronous Flushing
// <i> Please select this option when using asynchronous flushing, e.g. DMA + ISR 
#ifndef __DISP0_CFG_ENABLE_ASYNC_FLUSHING__
#   define __DISP0_CFG_ENABLE_ASYNC_FLUSHING__                     0
#endif

// <q>Enable the helper service for 3FB (LCD Direct Mode)
// <i> You can select this option when your LCD controller supports direct mode
#ifndef __DISP0_CFG_ENABLE_3FB_HELPER_SERVICE__
#   define __DISP0_CFG_ENABLE_3FB_HELPER_SERVICE__                 0
#endif

// <q>Disable the default scene
// <i> Remove the default scene for this display adapter. We highly recommend you to disable the default scene when creating real applications.
#ifndef __DISP0_CFG_DISABLE_DEFAULT_SCENE__
#   define __DISP0_CFG_DISABLE_DEFAULT_SCENE__                     0
#endif

// <o>Maximum number of Virtual Resources used per API
//     <0=>     NO Virtual Resource
//     <1=>     Background Loading Mode
//     <2=>     1 Per API
//     <3=>     2 Per API
//     <4=>     3 Per API
// <i> Introduce a helper service for loading virtual resources.
// <i> This feature is disabled by default.
// <i> NOTE: When selecting the background loading mode, you can ONLY use virtual resource as the source tile in the tile-copy-only APIs. 
#ifndef __DISP0_CFG_VIRTUAL_RESOURCE_HELPER__
#   define __DISP0_CFG_VIRTUAL_RESOURCE_HELPER__                   0
#endif

// <q>Use heap to allocate buffer in the virtual resource helper service
// <i> Use malloc and free in the virtual resource helper service. When disabled, a static buffer in the size of current display adapter PFB will be used. 
// <i> This feature is disabled by default.
#ifndef __DISP0_CFG_USE_HEAP_FOR_VIRTUAL_RESOURCE_HELPER__
#   define __DISP0_CFG_USE_HEAP_FOR_VIRTUAL_RESOURCE_HELPER__      0
#endif

// </h>

// <<< end of configuration section >>>

#if __DISP0_CFG_COLOR_SOLUTION__ == 1
/* the colour solution for monochrome screen */
#   undef __DISP0_CFG_COLOUR_DEPTH__
#   define __DISP0_CFG_COLOUR_DEPTH__                               8
#endif

#ifndef __DISP0_COLOUR_FORMAT__
#   if      __DISP0_CFG_COLOUR_DEPTH__ == 8
#       define __DISP0_COLOUR_FORMAT__  ARM_2D_COLOUR_GRAY8
#   elif    __DISP0_CFG_COLOUR_DEPTH__ == 16
#       define __DISP0_COLOUR_FORMAT__  ARM_2D_COLOUR_RGB565
#   elif    __DISP0_CFG_COLOUR_DEPTH__ == 32
#       define __DISP0_COLOUR_FORMAT__  ARM_2D_COLOUR_CCCN888
#   endif
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/

#if __DISP0_CFG_VIRTUAL_RESOURCE_HELPER__
#define disp_adapter0_impl_vres(__COLOUR_FORMAT, __WIDTH, __HEIGHT,...)         \
{                                                                               \
    .tTile = {                                                                  \
        .tRegion = {                                                            \
            .tSize = {                                                          \
                .iWidth = (__WIDTH),                                            \
                .iHeight =(__HEIGHT),                                           \
            },                                                                  \
        },                                                                      \
        .tInfo = {                                                              \
            .bIsRoot = true,                                                    \
            .bHasEnforcedColour = true,                                         \
            .bVirtualResource = true,                                           \
            .tColourInfo = {                                                    \
                .chScheme = (__COLOUR_FORMAT),                                  \
            },                                                                  \
        },                                                                      \
    },                                                                          \
    .Load       = &__disp_adapter0_vres_asset_loader,                           \
    .Depose     = &__disp_adapter0_vres_buffer_deposer,                         \
    __VA_ARGS__                                                                 \
}
#endif

#define disp_adapter0_task(...)                                                 \
        ({                                                                      \
        static bool ARM_2D_SAFE_NAME(s_bRefreshLCD) = false;                    \
        arm_fsm_rt_t ARM_2D_SAFE_NAME(ret) = arm_fsm_rt_on_going;               \
        if (!__ARM_VA_NUM_ARGS(__VA_ARGS__)) {                                  \
            ARM_2D_SAFE_NAME(ret) = __disp_adapter0_task();                     \
        } else {                                                                \
            if (!ARM_2D_SAFE_NAME(s_bRefreshLCD)) {                             \
                /* lock framerate */                                            \
                if (arm_2d_helper_is_time_out(1000 / (1000,##__VA_ARGS__))) {   \
                    ARM_2D_SAFE_NAME(s_bRefreshLCD) = true;                     \
                }                                                               \
            } else {                                                            \
                ARM_2D_SAFE_NAME(ret) = __disp_adapter0_task();                 \
                if (arm_fsm_rt_cpl == ARM_2D_SAFE_NAME(ret)) {                  \
                    ARM_2D_SAFE_NAME(s_bRefreshLCD) = false;                    \
                }                                                               \
            }                                                                   \
        };                                                                      \
        ARM_2D_SAFE_NAME(ret);})

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
ARM_NOINIT
extern
arm_2d_scene_player_t DISP0_ADAPTER;

/*============================ PROTOTYPES ====================================*/

extern
void disp_adapter0_init(void);

extern
arm_fsm_rt_t __disp_adapter0_task(void);


#if __DISP0_CFG_VIRTUAL_RESOURCE_HELPER__
/*!
 *  \brief a method to load a specific part of an image
 *  \note It is NOT an API for users to call, plese leave it alone
 *
 *  \param[in] pTarget a reference of an user object 
 *  \param[in] ptVRES a reference of this virtual resource
 *  \param[in] ptRegion the target region of the image
 *  \return intptr_t the address of a resource buffer which holds the content
 */
intptr_t __disp_adapter0_vres_asset_loader   (
                                                uintptr_t pTarget, 
                                                arm_2d_vres_t *ptVRES, 
                                                arm_2d_region_t *ptRegion);
    
/*!
 *  \brief a method to despose the buffer
 *  \note It is NOT an API for users to call, plese leave it alone
 *
 *  \param[in] pTarget a reference of an user object 
 *  \param[in] ptVRES a reference of this virtual resource
 *  \param[in] pBuffer the target buffer
 */
void __disp_adapter0_vres_buffer_deposer (  uintptr_t pTarget, 
                                                arm_2d_vres_t *ptVRES, 
                                                intptr_t pBuffer );

/*!
 * \brief A user implemented function to return the address for specific asset
 *        stored in external memory, e.g. SPI Flash
 * \note You MUST provide an implementation when 
 *       __DISP0_CFG_VIRTUAL_RESOURCE_HELPER__ is enabled(1)
 *
 * \param[in] pObj an pointer of user defined object, it is used for OOC
 * \note You can ignore pObj if you don't care/don't use OOC 
 *
 * \param[in] ptVRES the target virtual resource object
 * \return uintptr_t the address of the target asset in external memory
 */
extern
uintptr_t __disp_adapter0_vres_get_asset_address(
                                                        uintptr_t pObj,
                                                        arm_2d_vres_t *ptVRES);

/*!
 * \brief A user implemented function to copy content from external memory
 *        (e.g. SPI Flash) to a local buffer with specified address and size.
 *
 * \note You MUST provide an implementation when 
 *       __DISP0_CFG_VIRTUAL_RESOURCE_HELPER__ is enabled(1)
 *
 * \param[in] pObj an pointer of user defined object, it is used for OOC
 * \note You can ignore pObj if you don't care/don't use OOC 
 *
 * \param[in] pBuffer the address of the local buffer
 * \param[in] pAddress the address in the external memory
 * \param[in] nSizeInByte number of bytes to read
 */
extern
void __disp_adapter0_vres_read_memory( intptr_t pObj, 
                                                void *pBuffer,
                                                uintptr_t pAddress,
                                                size_t nSizeInByte);

#endif

#if __DISP0_CFG_ENABLE_ASYNC_FLUSHING__

#   if __DISP0_CFG_ENABLE_3FB_HELPER_SERVICE__

/*!
 * \brief An user implemented interface for DMA memory-to-memory copy.
 *        You should implement an ISR for copy-complete event and call
 *        disp_adapter0_insert_dma_copy_complete_event_handler() or
 *        arm_2d_helper_3fb_report_dma_copy_complete() to notify the 
 *        3FB (direct mode) helper service.
 * 
 * \param[in] ptThis the helper service control block
 * \param[in] pObj the address of the user object
 * \param[in] pnSource the source address of the memory block
 * \param[in] pnTarget the target address
 * \param[in] nDataItemCount the number of date items
 * \param[in] chDataItemSize the size of each data item 
 */
extern
void __disp_adapter0_request_dma_copy(  arm_2d_helper_3fb_t *ptThis,
                                        void *pObj,
                                        uintptr_t pnSource,
                                        uintptr_t pnTarget,
                                        uint32_t nDataItemCount,
                                        uint_fast8_t chDataItemSize);

/*!
 * \brief An user implemented interface for 2D-Copy.
 * \param[in] pnSource the source image address
 * \param[in] wSourceStride the stride of the source image
 * \param[in] pnTarget the address in the target framebuffer
 * \param[in] wTargetStride the stride of the target framebuffer
 * \param[in] iWidth the safe width of the source image
 * \param[in] iHeight the safe height of the source image
 * \retval true the 2D copy is complete when leaving this function
 * \retval false An async 2D copy request is sent to the DMA
 *
 * \note if false is replied, you have to call 
 *       disp_adapter0_insert_2d_copy_complete_event_handler() to report
 *       the completion of the 2d-copy. 
 */
bool __disp_adapter0_request_2d_copy(   arm_2d_helper_3fb_t *ptThis,
                                        void *pObj,
                                        uintptr_t pnSource,
                                        uint32_t wSourceStride,
                                        uintptr_t pnTarget,
                                        uint32_t wTargetStride,
                                        int16_t iWidth,
                                        int16_t iHeight,
                                        uint_fast8_t chBytePerPixel );

/*!
 * \brief the handler for the 2d copy complete event.
 * \note When both __DISP0_CFG_ENABLE_ASYNC_FLUSHING__ and 
 *       __DISP0_CFG_ENABLE_3FB_HELPER_SERVICE__ is set to '1', user 
 *       MUST call this function to notify the PFB helper that the previous
 *       asynchronouse 2d copy is complete. 
 * \note When people using DMA+ISR to offload CPU, this fucntion is called in 
 *       the DMA transfer complete ISR.
 */
extern
void disp_adapter0_insert_2d_copy_complete_event_handler(void);

/*!
 * \brief the handler for the dma copy complete event.
 * \note When both __DISP0_CFG_ENABLE_ASYNC_FLUSHING__ and 
 *       __DISP0_CFG_ENABLE_3FB_HELPER_SERVICE__ is set to '1', user 
 *       MUST call this function to notify the PFB helper that the previous
 *       dma copy is complete. 
 * \note When people using DMA+ISR to offload CPU, this fucntion is called in 
 *       the DMA transfer complete ISR.
 */
extern
void disp_adapter0_insert_dma_copy_complete_event_handler(void);

#   else

/*!
 * \brief It is an user implemented function that request an LCD flushing in 
 *        asynchronous manner. 
 * \note User MUST implement this function when 
 *       __DISP0_CFG_ENABLE_ASYNC_FLUSHING__ is set to '1'
 *
 * \param[in] pTarget an user specified object address
 * \param[in] bIsNewFrame whether this flushing request is the first iteration 
 *            of a new frame.
 * \param[in] iX the x coordinate of a flushing window in the target screen
 * \param[in] iY the y coordinate of a flushing window in the target screen
 * \param[in] iWidth the width of a flushing window
 * \param[in] iHeight the height of a flushing window
 * \param[in] pBuffer the frame buffer address
 */
extern void __disp_adapter0_request_async_flushing( 
                                                    void *pTarget,
                                                    bool bIsNewFrame,
                                                    int16_t iX, 
                                                    int16_t iY,
                                                    int16_t iWidth,
                                                    int16_t iHeight,
                                                    const COLOUR_INT *pBuffer);


/*!
 * \brief the handler for the asynchronous flushing complete event.
 * \note When __DISP0_CFG_ENABLE_ASYNC_FLUSHING__ is set to '1', user 
 *       MUST call this function to notify the PFB helper that the previous
 *       asynchronous flushing is complete. 
 * \note When people using DMA+ISR to offload CPU, this fucntion is called in 
 *       the DMA transfer complete ISR.
 */
extern
void disp_adapter0_insert_async_flushing_complete_event_handler(void);

#   endif
#endif

#if __DISP0_CFG_ENABLE_3FB_HELPER_SERVICE__

/*!
 * \brief get a pointer for flushing
 * \return void * the address of a framebuffer
 * 
 * \note please only call this function when on vsync event.
 */
extern
void *disp_adapter0_3fb_get_flush_pointer(void);

#endif


#if __DISP0_CFG_USE_CONSOLE__
extern
ARM_NONNULL(1)
int disp_adapter0_printf(const char *format, ...);

extern
bool disp_adapter0_putchar(uint8_t chChar);
#else
#   define disp_adapter0_printf(__format_string, ...)
#   define disp_adapter0_putchar(...)           (true)
#endif

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

#ifdef   __cplusplus
}
#endif

#endif

#endif
