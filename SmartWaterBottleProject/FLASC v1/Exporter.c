/*
 * Export.c
 *
 *  Created on: Nov 18, 2021
 *      Author: Dean Pickett
 *
 *      This file contains two functions: Export and uart_write_uint8. Export takes in the
 *      battery percentage and water quality reading as two uint8_t values. It then sends
 *      strings over UART to the BLE module to show the user both values with descriptions
 *      in shorthand. This limitation is to fit everything within the 20 byte payload of
 *      the data characteristics used by the Cypress BLE modules EZ-Serial Firmware.
 *      uart_write_uint8 is a helper function for Export that extracts the individual digits
 *      from the uint8_t values to transmit each digit via UART in ASCII format
 *
 *      The counter j is meant to keep track of how many bytes have been sent so that we can ensure
 *      the payload size of 20 bytes has been sent every time that Export is called.
 *      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *      I am currently having trouble getting the last byte of data to transmit to the phone properly
 */
#include <Def.h>
#include <Exporter.h>

void uart_write_uint8(uint8_t n, uint8_t* j);

//ValidSample =1, if analyzer was called, else 0 (if cap is removed after analyzer is called, sample is no longer valid)
void Export(uint8_t BattPerc, bool WatQual, bool ValidSample){

    unsigned char transmit1[6] = "Bat: ";
    unsigned char transmit2[15] = "% | WQ: Good ";  //Had to make slightly larger, dont know why
    unsigned char transmit3[12]  = "% | WQ: Bad";
    unsigned char transmit4[2]    = "%";
    uint8_t i=0, j=0;


    EUSCI_A_UART_enable(EUSCI_A0_BASE);

    // Enable USCI_A0 RX interrupt, taken from driverlib guide p.132
 //   EUSCI_A_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);

    //transmits the string "Bat: "
    for(i=0;i<5;i++){
        EUSCI_A_UART_transmitData (EUSCI_A0_BASE, transmit1[i]);
        j++;
    }

    uart_write_uint8(BattPerc, &j);

    if(!ValidSample)
    {
        //transmits the string "% | WQ: Good"
        for(i=0;i<1;i++)
            {
            EUSCI_A_UART_transmitData (EUSCI_A0_BASE, transmit4[i]);
            j++;
            }
    }


    if(ValidSample)
    {
        if(WatQual)
        {
            //transmits the string "% | WQ: Good"
            for(i=0;i<14;i++)
                {
                EUSCI_A_UART_transmitData (EUSCI_A0_BASE, transmit2[i]);
                j++;
                }
        }

        else if (!WatQual)
        {
            //transmits the string "% | WQ: Bad"
            for(i=0;i<11;i++)
            {
                EUSCI_A_UART_transmitData (EUSCI_A0_BASE, transmit3[i]);
                j++;
            }
        }
    }






//
//    uart_write_uint8(WatQual, &j);


//    EUSCI_A_UART_transmitData (EUSCI_A0_BASE, '% \n'); // percent sign for water quality reading
//    j++;

    //this is trying to fill out the rest of the Cypress data characteristic with spaces
    //if nothing is added to punc
   while(j<20){
        EUSCI_A_UART_transmitData (EUSCI_A0_BASE, 0x20); // hex value of ASCII space character, 0x20
        j++;
    }


    EUSCI_A_UART_disable (EUSCI_A0_BASE);

    return;
}

//this extracts the digits from a uint8_t and sends them via UART as an ASCII character
void uart_write_uint8(uint8_t n, uint8_t* j){
    uint8_t dig;

    if(n>=100){
            dig = (n/100)%10; // retrieve hundreds place digit from n
            EUSCI_A_UART_transmitData (EUSCI_A0_BASE, '0'+dig); //send digit over UART
            (*j)++;
        }
    if(n>=10){
            dig = (n/10)%10; // retrieve tens place digit from n
            EUSCI_A_UART_transmitData (EUSCI_A0_BASE, '0'+dig); //send digit over UART
            (*j)++;
        }
    dig = n%10; // retrieve ones place digit from n
        EUSCI_A_UART_transmitData (EUSCI_A0_BASE,'0'+dig); //send digit over UART
        (*j)++;

}
