// ppm_ev.h
// Evan Von Oehsen on November 6, 2020

#include <stdint.h>
#include "EasyREDVIO.h"
///////////////////////////////////////////////////////////////////////////////
// PWM Registers
///////////////////////////////////////////////////////////////////////////////

// Pin modes

// Declare a PWM structure defining the PWM registers in the order they appear in memory mapped I/O
typedef struct
{
    volatile uint32_t   pwmcfg;      // (PWM offset 0x00) Pin value
    volatile uint32_t   reserved_1;       // (PWM offset 0x04) Pin input enable*
    volatile uint32_t   pwmcount;      // (PWM offset 0x08) Pin output enable*
    volatile uint32_t   reserved_2;     // (PWM offset 0x0C) Output value
    volatile uint32_t   pwms;            // (PWM offset 0x10) Internal pull-up enable*
    volatile uint32_t   reserved_3;             // (PWM offset 0x14) Pin drive strength
    volatile uint32_t   reserved_4;        // (PWM offset 0x18) Rise interrupt enable
    volatile uint32_t   reserved_5;        // (PWM offset 0x1C) Rise interrupt pending
    volatile uint32_t   pwmcmp0;        // (PWM offset 0x20) Fall interrupt enable
    volatile uint32_t   pwmcmp1;        // (PWM offset 0x24) Fall interrupt pending
    volatile uint32_t   pwmcmp2;        // (PWM offset 0x28) High interrupt enable
    volatile uint32_t   pwmcmp3;        // (PWM offset 0x2C) High interrupt pending
} PWM;
// Define the base address of the PWM registers and a pointer to this structure
// The 0x…U notation in 0x10012000U indicates an unsigned hexadecimal number
#define PWM2_BASE   (0x10035000U) 
#define PWM2        ((PWM*) PWM2_BASE)



void pwmInit() {
    PWM2 -> pwmcfg = 0;
    PWM2 -> pwmcfg |= 0x00001200U; //set pwmenalways and zerocomnp
    pinMode(11, GPIO_IOF1); // double check this one
}

//sete pwmmcfg.scale, pwmcmp0 and pwmcmp1 registers to produce waveform with specified frequency and duty cycle.
//remember base clock freq is 16 MHz and pwmcmp0 and 1 registers are only 16 bits long, so to produce low frequency outputs the scale must be a nonzero. However setting a high scale gives a poor frequency resolution
void pwm(int freq, float duty) {
    if (freq != 0) {
        float period = 1.0 / (float) freq;
        float base_period = 0.0000000625;
        uint32_t num_chunks = (uint32_t) (period / base_period);
        uint8_t scale = 0;
        while (num_chunks > 65535) {
            num_chunks /= 2;
            scale ++;
        }
        PWM2 -> pwmcmp0 = num_chunks; //period
        PWM2 -> pwmcmp1 = (uint32_t) ((float) num_chunks * (1.0 - duty));
        PWM2 -> pwmcfg |= scale;
    } else {
        duty = 0.0;
        PWM2 -> pwmcmp0 = 1;
        PWM2 -> pwmcmp1 = 1;
        
        // config register |= first three bits
    }
}

