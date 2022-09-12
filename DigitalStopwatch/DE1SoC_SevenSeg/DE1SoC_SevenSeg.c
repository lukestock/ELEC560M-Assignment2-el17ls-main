/*
 * DE1SoC_SevenSeg.c
 *
 *  Created on: 12 Feb 2021
 *      Author: Harry Clegg
 *      		Luke Stock
 */

#include "DE1SoC_SevenSeg.h"

// ToDo: Add the base addresses of the seven segment display peripherals.
volatile unsigned char *sevenseg_base_lo_ptr = (unsigned char *)0xFF200020;  // HEX0, HEX1, HEX2 and HEX3 peripheral base address
volatile unsigned char *sevenseg_base_hi_ptr = (unsigned char *)0xFF200030;  // HEX4 and HEX5 peripheral base address

// There are four HEX displays attached to the low (first) address.
#define SEVENSEG_N_DISPLAYS_LO 4

// There are two HEX displays attached to the high (second) address.
#define SEVENSEG_N_DISPLAYS_HI 2

void DE1SoC_SevenSeg_Write(unsigned int display, unsigned char value) {
    // Select between the two addresses so that the higher level functions
    // have a seamless interface.
    if (display < SEVENSEG_N_DISPLAYS_LO) {
        // If we are targeting a low address, use byte addressing to access
        // directly.
        sevenseg_base_lo_ptr[display] = value;
    } else {
        // If we are targeting a high address, shift down so byte addressing
        // works.
        display = display - SEVENSEG_N_DISPLAYS_LO;
        sevenseg_base_hi_ptr[display] = value;
    }
}

void DE1SoC_SevenSeg_SetSingle(unsigned int display, unsigned int value) {
    // ToDo: Write the code for driving a single seven segment display here.
    // Your function should turn a real value 0-F into the correctly encoded
    // bits to enable the correct segments on the seven segment display to
    // illuminate. Use the DE1SoC_SevenSeg_Write function you created earlier
    // to set the bits of the display.
	if (value == 0) {
		value = 0x3F;          // 0
	} else if (value == 1) {
		value = 0x06;          // 1
	} else if (value == 2) {
		value = 0x5B;          // 2
	} else if (value == 3) {
		value = 0x4F;          // 3
	} else if (value == 4) {
		value = 0x66;          // 4
	} else if (value == 5) {
		value = 0x6D;          // 5
	} else if (value == 6) {
		value = 0x7D;          // 6
	} else if (value == 7) {
		value = 0x07;          // 7
	} else if (value == 8) {
		value = 0x7F;          // 8
	} else if (value == 9) {
		value = 0x67;          // 9
	} else if (value == 0xA) {
		value = 0x77;          // A
	} else if (value == 0xB) {
		value = 0x7C;          // B
	} else if (value == 0xC) {
		value = 0x39;          // C
	} else if (value == 0xD) {
		value = 0x5E;          // D
	} else if (value == 0xE) {
		value = 0x79;          // E
	} else if (value == 0xF) {
		value = 0x71;          // F
	} else {
		value = 0x40;          // -
	}

	DE1SoC_SevenSeg_Write(display, value);
}

void DE1SoC_SevenSeg_SetDoubleHex(unsigned int display, unsigned int value) {
    // ToDo: Write the code for setting a pair of seven segment displays
    // here. Good coding practice suggests your solution should call
    // DE1SoC_SevenSeg_SetSingle() twice.
    // This function should show the first digit of a HEXADECIMAL number on
    // the specified 'display', and the second digit on the display to
    // the left of the specified display.

    /** Some examples:
     *
     *    input | output | HEX(N+1) | HEX(N)
     *    ----- | ------ | -------- | ------
     *        5 |     05 |        0 |      5
     *       30 |     1E |        1 |      E
     *     0x60 |     60 |        6 |      0
     */
	unsigned int lower_digit;
	unsigned int higher_digit;

	// Set the 4 MSB of the byte to 0 to obtain the lower digit
	if (value <= 0xFF) {

		lower_digit = value & 0xF;

		if (lower_digit == 0) {
			value = 0x3F;          // 0
		} else if (lower_digit == 1) {
			value = 0x06;          // 1
		} else if (lower_digit == 2) {
			value = 0x5B;          // 2
		} else if (lower_digit == 3) {
			value = 0x4F;          // 3
		} else if (lower_digit == 4) {
			value = 0x66;          // 4
		} else if (lower_digit == 5) {
			value = 0x6D;          // 5
		} else if (lower_digit == 6) {
			value = 0x7D;          // 6
		} else if (lower_digit == 7) {
			value = 0x07;          // 7
		} else if (lower_digit == 8) {
			value = 0x7F;          // 8
		} else if (lower_digit == 9) {
			value = 0x67;          // 9
		} else if (lower_digit == 0xA) {
			value = 0x77;          // A
		} else if (lower_digit == 0xB) {
			value = 0x7C;          // B
		} else if (lower_digit == 0xC) {
			value = 0x39;          // C
		} else if (lower_digit == 0xD) {
			value = 0x5E;          // D
		} else if (lower_digit == 0xE) {
			value = 0x79;          // E
		} else if (lower_digit == 0xF) {
			value = 0x71;          // F
		}

		higher_digit = (value & 0xF0) >> 4;

	}
	else {

		lower_digit = 0x40;
		higher_digit = 0x40;
	}


	// 4 bit shift right to obtain the higher digit in the 4 LSB
	// bits of the byte
	higher_digit = ((value & 0xF) >> 4);

	DE1SoC_SevenSeg_SetSingle(display, lower_digit);
	DE1SoC_SevenSeg_SetSingle(display+1,higher_digit);

}

void DE1SoC_SevenSeg_SetDoubleDec(unsigned int display, unsigned int value) {
    // ToDo: Write the code for setting a pair of seven segment displays
    // here. Good coding practice suggests your solution should call
    // DE1SoC_SevenSeg_SetSingle() twice.
    // This function should show the first digit of a DECIMAL number on
    // the specified 'display', and the second digit on the display to
    // the left of the specified display.

    /** Some examples:
     *
     *	  input | output | HEX(N+1) | HEX(N)
     *    ----- | ------ | -------- | ------
     *        5 |     05 |        0 |      5
     *       30 |     30 |        3 |      0
     *     0x90 |     96 |        9 |      6
     */
	// Split value read (0-99) into the decade and unit values to be
	// displayed on two adjacent displays.
	unsigned int decades;
	unsigned int units;

	if (value >= 100) {
		units = 0;
		decades = 0;
	} else if (value >= 10) {
		decades = value / 10;
		units = value % 10;
	} else if (value < 10) {
		decades = 0;
		units = value;
	}
	DE1SoC_SevenSeg_SetSingle(display, units);
	DE1SoC_SevenSeg_SetSingle(display+1,decades);
}
