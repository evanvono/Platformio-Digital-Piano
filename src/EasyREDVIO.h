// EasyREDVIO.h
// Joshua Brake and David Harris 7 October 2020

#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////
// GPIO Registers
///////////////////////////////////////////////////////////////////////////////

// Pin modes
#define INPUT 0
#define OUTPUT 1
#define GPIO_IOF0 2
#define GPIO_IOF1 3

// Declare a GPIO structure defining the GPIO registers in the order they appear in memory mapped I/O
typedef struct
{
    volatile uint32_t   input_val;      // (GPIO offset 0x00) Pin value
    volatile uint32_t   input_en;       // (GPIO offset 0x04) Pin input enable*
    volatile uint32_t   output_en;      // (GPIO offset 0x08) Pin output enable*
    volatile uint32_t   output_val;     // (GPIO offset 0x0C) Output value
    volatile uint32_t   pue;            // (GPIO offset 0x10) Internal pull-up enable*
    volatile uint32_t   ds;             // (GPIO offset 0x14) Pin drive strength
    volatile uint32_t   rise_ie;        // (GPIO offset 0x18) Rise interrupt enable
    volatile uint32_t   rise_ip;        // (GPIO offset 0x1C) Rise interrupt pending
    volatile uint32_t   fall_ie;        // (GPIO offset 0x20) Fall interrupt enable
    volatile uint32_t   fall_ip;        // (GPIO offset 0x24) Fall interrupt pending
    volatile uint32_t   high_ie;        // (GPIO offset 0x28) High interrupt enable
    volatile uint32_t   high_ip;        // (GPIO offset 0x2C) High interrupt pending
    volatile uint32_t   low_ie;         // (GPIO offset 0x30) Low interrupt enable
    volatile uint32_t   low_ip;         // (GPIO offset 0x34) Low interrupt pending
    volatile uint32_t   iof_en;         // (GPIO offset 0x38) HW-Driven functions enable
    volatile uint32_t   iof_sel;        // (GPIO offset 0x3C) HW-Driven functions selection
    volatile uint32_t   out_xor;        // (GPIO offset 0x40) Output XOR (invert)
} GPIO;
// Define the base address of the GPIO registers and a pointer to this structure
// The 0x…U notation in 0x10012000U indicates an unsigned hexadecimal number
#define GPIO0_BASE   (0x10012000U) 
#define GPIO0        ((GPIO*) GPIO0_BASE)

///////////////////////////////////////////////////////////////////////////////
// SPI Registers
///////////////////////////////////////////////////////////////////////////////

typedef struct
{
    volatile uint32_t   div         :   12; // Divisor for serial clock. div_width bits wide
    volatile uint32_t               :   20;
} sckdiv_bits;

typedef struct
{
    volatile uint32_t   pha         :   1; // Serial clock phase
    volatile uint32_t   pol         :   1; // Serial clock polarity
    volatile uint32_t               :   30;
} sckmode_bits;

typedef struct
{
    volatile uint32_t   csid        :   32; // Chip select ID. log2(cs_width) bits wide.
} csid_bits;

typedef struct
{
    volatile uint32_t   csdef       :   32; // Chip select default value. cs_widthbits wide, reset to all 1s.
} csdef_bits;

typedef struct
{
    volatile uint32_t   mode        :   2; // Chip select mode
    volatile uint32_t               :   30;
} csmode_bits;

typedef struct
{
    volatile uint32_t   cssck       :   8; // CS to SCK Delay
    volatile uint32_t               :   8;
    volatile uint32_t   sckcs       :   8; // SCK to CS Delay
    volatile uint32_t               :   8;
} delay0_bits;

typedef struct
{
    volatile uint32_t   intercs     :   8; // Minimum CS inactive time
    volatile uint32_t               :   8;
    volatile uint32_t   interxfr    :   8; // Maximum interframe delay
    volatile uint32_t               :   8;
} delay1_bits;

typedef struct
{
    volatile uint32_t   proto       :   2; // SPI protocol
    volatile uint32_t   endian      :   1; // SPI endianness
    volatile uint32_t   dir         :   1; // SPI I/O direcation.
    volatile uint32_t               :   12;
    volatile uint32_t   len         :   4; // Number of bits per frame
    volatile uint32_t               :   12;
} fmt_bits;

typedef struct
{
    volatile uint32_t   data        :   8; // Transmit data
    volatile uint32_t               :   23;
    volatile uint32_t   full        :   1; // FIFO full flag
} txdata_bits;

typedef struct
{
    volatile uint32_t   data        :   8; // Received data
    volatile uint32_t               :   23;
    volatile uint32_t   empty       :   1; // FIFO empty flag
} rxdata_bits;

typedef struct
{
    volatile uint32_t   txmark      :   3; // Transmit watermark
    volatile uint32_t               :   29;
} txmark_bits;

typedef struct
{
    volatile uint32_t   rxmark      :   3; // Receive watermark
    volatile uint32_t               :   29;
} rxmark_bits;

typedef struct
{
    volatile uint32_t   en          :   1; // SPI Flash Mode Select
    volatile uint32_t               :   31;
} fctrl_bits;

typedef struct
{
    volatile uint32_t   cmd_en      :   1; // Enable sending of command
    volatile uint32_t   addr_len    :   3; // Number of address bytes (0 to 4)
    volatile uint32_t   pad_cnt     :   4; // Number of dummy cycles
    volatile uint32_t   cmd_proto   :   2; // Protocol for transmitting command
    volatile uint32_t   addr_proto  :   2; // Protocol for transmitting address and padding
    volatile uint32_t   data_proto  :   2; // Protocol for receiving data bytes
    volatile uint32_t               :   2;
    volatile uint32_t   cmd_code    :   8; // Value of command byte
    volatile uint32_t   pad_code    :   8; // First 8 bits to transmit during dummy cycles
} ffmt_bits;

typedef struct
{
    volatile uint32_t   txwm        :   1; // Transmit watermark enable
    volatile uint32_t   rxwm        :   1; // Receive watermark enable
    volatile uint32_t               :   30;
} ie_bits;

typedef struct
{
    volatile uint32_t   txwm        :   1; // Transmit watermark pending
    volatile uint32_t   rxwm        :   1; // Receive watermark pending
    volatile uint32_t               :   30;
} ip_bits;

typedef struct
{
    volatile sckdiv_bits    sckdiv;         // (SPI offset 0x00) Serial clock divisor
    volatile sckmode_bits   sckmode;        // (SPI offset 0x04) Serial clock mode
    volatile uint32_t       Reserved1[2];
    volatile csid_bits      csid;           // (SPI offset 0x10) Chip select ID
    volatile csdef_bits     csdef;          // (SPI offset 0x14) Chip select default
    volatile csmode_bits    csmode;         // (SPI offset 0x18) Chip select mode
    volatile uint32_t       Reserved2[3];
    volatile delay0_bits    delay0;         // (SPI offset 0x28) Delay control 0
    volatile delay1_bits    delay1;         // (SPI offset 0x2C) Delay control 1
    volatile uint32_t       Reserved3[4];
    volatile fmt_bits       fmt;            // (SPI offset 0x40) Frame format
    volatile uint32_t       Reserved4[1];
    volatile txdata_bits    txdata;         // (SPI offset 0x48) Tx FIFO data
    volatile rxdata_bits    rxdata;         // (SPI offset 0x4C) Rx FIFO data
    volatile txmark_bits    txmark;         // (SPI offset 0x50) Tx FIFO watermark
    volatile rxmark_bits    rxmark;         // (SPI offset 0x54) Rx FIFO watermark
    volatile uint32_t       Reserved5[2];
    volatile fctrl_bits     fctrl;          // (SPI offset 0x60) SPI flash control*
    volatile ffmt_bits      ffmt;           // (SPI offset 0x64) SPI flash instr format*
    volatile uint32_t       Reserved6[2];
    volatile ie_bits        ie;             // (SPI offset 0x70) SPI interrupt enable
    volatile ip_bits        ip;             // (SPI offset 0x74) SPI interrupt pending
    // Registers marked * are only present on controllers with the direct-map flash ifc.

} SPI;

#define QSPI0_BASE  (0x10014000U)   // QSPI0 memory-mapped base address
#define SPI1_BASE   (0x10024000U)   // SPI1 memory-mapped base address
#define SPI2_BASE   (0x10034000U)   // SPI2 memory-mapped base address

#define QSPI0 ((SPI*) QSPI0_BASE)  // Set up pointer to struct of type SPI aligned at the base QSPI0 memory-mapped address
#define SPI1  ((SPI*) SPI1_BASE)  // Set up pointer to struct of type SPI aligned at the base SPI1 memory-mapped address
#define SPI2  ((SPI*) SPI2_BASE)  // Set up pointer to struct of type SPI aligned at the base SPI2 memory-mapped address

///////////////////////////////////////////////////////////////////////////////
// MTIME Register
///////////////////////////////////////////////////////////////////////////////

volatile uint64_t *mtime = (uint64_t*)0x0200bff8;
#define MTIME_CLK_FREQ 32768

///////////////////////////////////////////////////////////////////////////////
// I/O Functions
///////////////////////////////////////////////////////////////////////////////

void pinMode(int gpio_pin, int function)
{
    switch(function) {
        case INPUT:
            GPIO0->input_en     |= (1 << gpio_pin);   // Sets a pin as an input
            GPIO0->iof_en       &= ~(1 << gpio_pin);  // Disable IOF
            break;
        case OUTPUT:
            GPIO0->output_en    |= (1 << gpio_pin);   // Set pin as an output
            GPIO0->iof_en       &= ~(1 << gpio_pin);  // Disable IOF
            break;
        case GPIO_IOF0:
            GPIO0->iof_en       |=  (1 << gpio_pin);  // Enable IOF
            GPIO0->iof_sel      &= ~(1 << gpio_pin);  // IO Function 0
            break;    
        case GPIO_IOF1:
            GPIO0->iof_en       |=  (1 << gpio_pin);  // Enable IOF
            GPIO0->iof_sel      |=  (1 << gpio_pin);  // IO Function 1
            break;       
    }
}

void digitalWrite(int gpio_pin, int val)
{
    if (val) GPIO0->output_val |= (1 << gpio_pin);
    else     GPIO0->output_val &= ~(1 << gpio_pin);
}

int digitalRead(int gpio_pin)
{
    return (GPIO0->input_val >> gpio_pin) & 0x1;
}

void delay(int ms) {
    uint64_t doneTime = *mtime + (ms*MTIME_CLK_FREQ)/1000;
    while (*mtime < doneTime); // wait until time is reached
}

void spiInit(uint32_t clkdivide, uint32_t cpol, uint32_t cpha) {
    pinMode(2, GPIO_IOF0); // CS0 
    pinMode(3, GPIO_IOF0); // MOSI
    pinMode(4, GPIO_IOF0); // MISO
    pinMode(5, GPIO_IOF0); // SCK

    SPI1->sckdiv.div = clkdivide; // Set the clock divisor
    SPI1->sckmode.pol = cpol; // Set the polarity
    SPI1->sckmode.pha = cpha; // Set the phase
}

uint8_t spiSendReceive(uint8_t send) {
    while(SPI1->txdata.full); // Wait until transmit FIFO is ready for new data
    SPI1->txdata.data = send; // Transmit the character over SPI
    
    rxdata_bits rxdata;
    while (1) {
        rxdata = SPI1->rxdata;    // Read the rxdata register EXACTLY once
        if (!rxdata.empty) {    // If the empty bit was not set, return the data
            return (uint8_t)rxdata.data;
        }
    }
}

