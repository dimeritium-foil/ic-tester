#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#include "ic_data.h"
#include "uart.h"

char* test_ic();

volatile uint8_t* get_port(char port_char);
volatile uint8_t* get_ddr(char port_char);
volatile uint8_t* get_pin_reg(char port_char);
uint8_t compare_outputs(uint8_t output[], uint8_t expected_output[], uint8_t length);

volatile uint8_t debug_mode = 0;

int main() {
    
    // init button, set input pullup
    DDRC &= ~(1 << PINC0);
    PORTC |= (1 << PINC0);

    char* ic_match;

    // boolean for button state
    uint8_t button_is_pressed = 0;

    uart_init();

    while (1) {

        if (bit_is_clear(PINC, PINC0)) {
            if (button_is_pressed == 0) {

                uart_disable();
                ic_match = test_ic();
                uart_enable();

                if (ic_match == NULL) {
                    uart_transmit_string("null\n\r");
                }
                else {
                    uart_transmit_string(ic_match);
                    uart_transmit_string("\n\r");
                }

                button_is_pressed = 1;
            }
        }
        else {
            button_is_pressed = 0;
        }
    }
}

char* test_ic() {

    // variables for the registers bec they're not compile time constants
    volatile uint8_t* ddr;
    volatile uint8_t* port;

    uint8_t pin_val;
    uint8_t outputs[MAX_OUTPUT_PINS];
    uint8_t successful_tests;

    LogicIC ic;

    for (int i = 0; i < ic_table_length; i++) {

        ic = *ic_table_ptrs[i];

        // setting up the gnd pin
        ddr = get_ddr(ic.gnd_pin[0]);
        port = get_port(ic.gnd_pin[0]);
        *ddr |= (1 << (ic.gnd_pin[1] - '0'));
        *port &= ~(1 << (ic.gnd_pin[1] - '0'));

        // setting up the vcc pin
        ddr = get_ddr(ic.vcc_pin[0]);
        port = get_port(ic.vcc_pin[0]);
        *ddr |= (1 << (ic.vcc_pin[1] - '0'));
        *port |= (1 << (ic.vcc_pin[1] - '0'));

        successful_tests = 0;

        // now we're going to loop over the number of test cases we have
        for (int test = 0; test < TEST_INPUTS; test++) {

            if (ic.is_sequential) {
                ddr = get_ddr(ic.clk_pin[0]);
                port = get_port(ic.clk_pin[0]);
                *ddr |= (1 << (ic.clk_pin[1] - '0'));

                // falling edge
                if (ic.edge_detection == 0) {
                    *port &= ~(1 << (ic.clk_pin[1] - '0'));
                }
                // rising edge
                else {
                    *port |= (1 << (ic.clk_pin[1] - '0'));
                }

                _delay_ms(50);
            }

            // applying each input pin
            for (int pin = 0; pin < ic.inputs_num; pin++) {

                // set each corresponding pin the output
                port = get_port(ic.input_pins[pin][0]);
                ddr = get_ddr(ic.input_pins[pin][0]);
                *ddr |= (1 << (ic.input_pins[pin][1] - '0'));

                pin_val = ic.test_inputs[test][pin];
                switch (pin_val) {
                    case 1:
                        *port |= (1 << (ic.input_pins[pin][1] - '0'));
                        break;
                    case 0:
                        *port &= ~(1 << (ic.input_pins[pin][1] - '0'));
                        break;
                }
            }

            // give some time for the inputs and/or the clock signal to settle
            _delay_ms(10);

            if (ic.is_sequential) {
                ddr = get_ddr(ic.clk_pin[0]);
                port = get_port(ic.clk_pin[0]);
                *ddr |= (1 << (ic.clk_pin[1] - '0'));

                // falling edge
                if (ic.edge_detection == 0) {
                    *port |= (1 << (ic.clk_pin[1] - '0'));
                }
                // rising edge
                else {
                    *port &= ~(1 << (ic.clk_pin[1] - '0'));
                }

                _delay_ms(40);
            }

            // read each output pin into the outputs array
            for (int pin = 0; pin < ic.outputs_num; pin++) {

                // set each corresponding  pin to input so we can read the output, and disable input pullup
                ddr = get_ddr(ic.output_pins[pin][0]);
                port = get_port(ic.output_pins[pin][0]);
                *ddr &= ~(1 << (ic.output_pins[pin][1] - '0'));
                *port &= ~(1 << (ic.output_pins[pin][1] - '0'));

                if (bit_is_clear(*get_pin_reg(ic.output_pins[pin][0]), ic.output_pins[pin][1] - '0')) {
                    outputs[pin] = 0;
                }
                else {
                    outputs[pin] = 1;
                }
            }

            if (debug_mode) {
                uart_enable();
                uart_transmit_string("[ ");
                for (int j = 0; j < ic.outputs_num; j++) {
                    uart_transmit(outputs[j] + '0');
                    uart_transmit(' ');
                }
                uart_transmit_string("]\n\r");
                uart_disable();
            }

            if (compare_outputs(outputs, ic.expected_outputs[test], ic.outputs_num)) {
                successful_tests++;
            }
        }

        // if we passed all tests cases then we found a match
        if (successful_tests == TEST_INPUTS) {
            return ic.id;
        }
    }

    return NULL;
}

volatile uint8_t* get_port(char port_char) {

    switch (port_char) {
        case 'A':
            return &PORTA;
        case 'B':
            return &PORTB;
        case 'C':
            return &PORTC;
        case 'D':
            return &PORTD;
    }

    return NULL;
}

volatile uint8_t* get_ddr(char port_char) {

    switch (port_char) {
        case 'A':
            return &DDRA;
        case 'B':
            return &DDRB;
        case 'C':
            return &DDRC;
        case 'D':
            return &DDRD;
    }

    return NULL;
}

volatile uint8_t* get_pin_reg(char port_char) {

    switch (port_char) {
        case 'A':
            return &PINA;
        case 'B':
            return &PINB;
        case 'C':
            return &PINC;
        case 'D':
            return &PIND;
    }

    return NULL;
}

uint8_t compare_outputs(uint8_t output[], uint8_t expected_output[], uint8_t length) {

    for (int i = 0; i < length; i++) {
        if (output[i] != expected_output[i])
            return 0;
    }

    return 1;
}
