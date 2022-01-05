#ifndef IC_DATA_H
#define IC_DATA_H

/*
 * test IC pin connections:
 *
 *     PD0  PD1  PD2  PD3  PD4  PD5  PD6  
 *      |    |    |    |    |    |    |
 *  +-------------------------------------+
 *  |                                     |
 *  |D                                    |
 *  |                                     |
 *  +-------------------------------------+
 *      |    |    |    |    |    |    |
 *     PA0  PA1  PA2  PA3  PA4  PA5  PA6  
 *
 */

#include <avr/io.h>
#include <stdint.h>

#define MAX_INPUT_PINS 14
#define MAX_OUTPUT_PINS 14

#define TEST_INPUTS 6

typedef struct LogicIC {
    char*   id;

    char gnd_pin[2];
    char vcc_pin[2];

    uint8_t inputs_num;
    uint8_t outputs_num;

    char input_pins[MAX_INPUT_PINS][2];
    char output_pins[MAX_OUTPUT_PINS][2];

    uint8_t test_inputs[TEST_INPUTS][MAX_INPUT_PINS];
    uint8_t expected_outputs[TEST_INPUTS][MAX_OUTPUT_PINS];

    uint8_t is_sequential;
    char clk_pin[2];

    // 0 for falling edge, 1 for rising edge
    uint8_t edge_detection;
} LogicIC;

// decade and binary counter
volatile LogicIC ic_7490 = {
                            .id = "7490",

                            .gnd_pin = "D4",
                            .vcc_pin = "A4",

                            //         A
                            .clk_pin = "D0",
                            .is_sequential = 1,
                            .edge_detection = 0,

                            .inputs_num = 5,
                            .outputs_num = 4,

                            //             B     R01   R02   R91   R92
                            .input_pins = {"A0", "A1", "A2", "A5", "A6"},

                            //              QD    QC    QB    QA
                            .output_pins = {"D3", "D6", "D5", "D2"},

                            .test_inputs = {
                                            {0, 0, 0, 1, 1},
                                            {0, 0, 0, 0, 0},
                                            {0, 0, 0, 0, 0},
                                            {0, 0, 0, 0, 0},
                                            {0, 0, 0, 0, 0},
                                            {0, 0, 0, 0, 0},
                                           },
                            .expected_outputs = {
                                                 {1, 0, 0, 1},
                                                 {1, 0, 0, 1},
                                                 {1, 0, 0, 0},
                                                 {1, 0, 0, 1},
                                                 {1, 0, 0, 0},
                                                 {1, 0, 0, 1}
                                                },
                           };

// quad 2 input xor gates
volatile LogicIC ic_7486 = {
                            .id = "7486",

                            .gnd_pin = "A6",
                            .vcc_pin = "D0",

                            .inputs_num = 8,
                            .outputs_num = 4,

                            .input_pins = {"D1", "D2", "D4", "D5", "A0", "A1", "A3", "A4"},
                            .output_pins = {"D3", "D6", "A2", "A5"},

                            .test_inputs = {
                                            {1, 0, 0, 1, 1, 1, 0, 1},
                                            {1, 0, 0, 0, 0, 1, 0, 0},
                                            {0, 1, 0, 1, 1, 0, 0, 0},
                                            {1, 0, 1, 0, 0, 1, 1, 1},
                                            {0, 0, 0, 1, 0, 0, 1, 0},
                                            {1, 0, 1, 0, 1, 0, 1, 0}
                                           },
                            .expected_outputs = {
                                                 {1, 1, 0, 1},
                                                 {1, 0, 1, 0},
                                                 {1, 1, 1, 0},
                                                 {1, 1, 1, 0},
                                                 {0, 1, 0, 1},
                                                 {1, 1, 1, 1}
                                                },

                            .is_sequential = 0
                           };

// quad 2 input nand gates
volatile LogicIC ic_7400 = {
                            .id = "7400",

                            .gnd_pin = "A6",
                            .vcc_pin = "D0",

                            .input_pins = {"D1", "D2", "D4", "D5", "A0", "A1", "A3", "A4"},
                            .output_pins = {"D3", "D6", "A2", "A5"},

                            .inputs_num = 8,
                            .outputs_num = 4,

                            .test_inputs = {
                                            {1, 1, 0, 1, 0, 1, 1, 1},
                                            {0, 0, 1, 1, 0, 1, 0, 1},
                                            {1, 0, 1, 0, 0, 0, 0, 1},
                                            {0, 0, 1, 1, 1, 0, 0, 0},
                                            {1, 0, 1, 0, 0, 0, 1, 0},
                                            {1, 1, 1, 1, 1, 1, 0, 0}
                                           },
                            .expected_outputs = {
                                                 {0, 1, 1, 0},
                                                 {1, 0, 1, 1},
                                                 {1, 1, 1, 1},
                                                 {1, 0, 1, 1},
                                                 {1, 1, 1, 1},
                                                 {0, 0, 0, 1}
                                                },

                            .is_sequential = 0
                           };

// hex inverting gates
volatile LogicIC ic_7404 = {
                            .id = "7404",

                            .gnd_pin = "A6",
                            .vcc_pin = "D0",

                            .inputs_num = 6,
                            .outputs_num = 6,

                            .input_pins = {"D1", "D3", "D5", "A0", "A2", "A4"},
                            .output_pins = {"D2", "D4", "D6", "A1", "A3", "A5"},

                            .test_inputs = {
                                            {1, 1, 0, 1, 1, 0},
                                            {0, 1, 0, 1, 1, 1},
                                            {0, 0, 1, 0, 1, 0},
                                            {1, 1, 0, 1, 0, 1},
                                            {0, 1, 0, 1, 0, 1},
                                            {1, 1, 0, 1, 0, 0}
                                           },
                            .expected_outputs = {
                                                 {0, 0, 1, 0, 0, 1},
                                                 {1, 0, 1, 0, 0, 0},
                                                 {1, 1, 0, 1, 0, 1},
                                                 {0, 0, 1, 0, 1, 0},
                                                 {1, 0, 1, 0, 1, 0},
                                                 {0, 0, 1, 0, 1, 1}
                                                },

                            .is_sequential = 0
                           };

// quad 2 input or gates
volatile LogicIC ic_7432 = {
                            .id = "7432",

                            .gnd_pin = "A6",
                            .vcc_pin = "D0",

                            .inputs_num = 8,
                            .outputs_num = 4,

                            .input_pins = {"D1", "D2", "D4", "D5", "A0", "A1", "A3", "A4"},
                            .output_pins = {"D3", "D6", "A2", "A5"},

                            .test_inputs = {
                                            {1, 1, 0, 1, 1, 0, 0, 0},
                                            {0, 0, 0, 1, 1, 0, 0, 0},
                                            {0, 0, 1, 0, 1, 0, 0, 0},
                                            {1, 0, 0, 0, 0, 1, 0, 0},
                                            {1, 1, 0, 1, 0, 0, 1, 0},
                                            {1, 0, 1, 1, 0, 1, 0, 0}
                                           },
                            .expected_outputs = {
                                                 {1, 1, 1, 0},
                                                 {0, 1, 1, 0},
                                                 {0, 1, 1, 0},
                                                 {1, 0, 1, 0},
                                                 {1, 1, 0, 1},
                                                 {1, 1, 1, 0}
                                                },

                            .is_sequential = 0
                           };

volatile LogicIC* ic_table_ptrs[] = {&ic_7490, &ic_7486, &ic_7400, &ic_7404, &ic_7432};
volatile uint8_t ic_table_length = sizeof(ic_table_ptrs) / sizeof(ic_table_ptrs[0]);

#endif
