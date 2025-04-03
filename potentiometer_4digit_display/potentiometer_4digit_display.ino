/*
 * Project: Potentiometer-Controlled 4-Digit 7-Segment Display
 * Board: Arduino Uno
 * Device: HiLetGo Multi-Function Shield
 * Purpose: 
 * This program demonstrates how to use an analog potentiometer to control 
 * the values displayed on a 4-digit, 7-segment display. It utilizes shift 
 * registers to drive the display efficiently while reading the potentiometer 
 * value to dynamically update the digits. This code serves as a practical 
 * example of combining analog input processing with digital output control.
 * 
 * Features:
 * - Converts the potentiometer analog input (0–1023) into a 4-digit numerical output.
 * - Sends data to the 7-segment display using shift registers for optimized control.
 * - Includes detailed comments for better understanding and learning.
 * 
 * Usage:
 * This sketch is ideal for beginners and enthusiasts seeking to learn 
 * about interfacing with 7-segment displays and analog sensors in Arduino projects.
 */

// Define shift register pins used for the seven-segment display
#define LATCH_DIO 4  // Latch pin (ST_CP) of the shift register
#define CLK_DIO 7    // Clock pin (SH_CP) of the shift register
#define DATA_DIO 8   // Data pin (DS) of the shift register

#define Pot1 A0      // Analog input pin connected to the potentiometer

// Segment byte maps for numbers 0 to 9 (common cathode)
// Each byte represents the on/off state of segments a-g and dp for a 7-segment display
const byte SEGMENT_MAP[] = {
  0xC0, // 0
  0xF9, // 1
  0xA4, // 2
  0xB0, // 3
  0x99, // 4
  0x92, // 5
  0x82, // 6
  0xF8, // 7
  0x80, // 8
  0x90  // 9
};

// Byte maps to select digit 1 to 4 (active-low configuration)
// Each byte activates a specific digit on the display
const byte SEGMENT_SELECT[] = {
  0xF1, // Activate digit 1
  0xF2, // Activate digit 2
  0xF4, // Activate digit 3
  0xF8  // Activate digit 4
};

/**
 * Writes a decimal number (0-9) to a specific digit on the 7-segment display.
 *
 * @param Segment The index of the digit to activate (0-3).
 * @param Value The value to display on the digit (0-9).
 */
void WriteNumberToSegment(byte Segment, byte Value) {
  digitalWrite(LATCH_DIO, LOW); // Open the latch to start sending data

  // Send the segment data for the digit
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_MAP[Value]);

  // Send the digit select data to activate the appropriate digit
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_SELECT[Segment]);

  digitalWrite(LATCH_DIO, HIGH); // Close the latch to update the display
}

void setup() {
  // Set the shift register control pins as outputs
  pinMode(LATCH_DIO, OUTPUT);
  pinMode(CLK_DIO, OUTPUT);
  pinMode(DATA_DIO, OUTPUT);

  // Initialize the serial monitor for debugging (optional)
  Serial.begin(9600);
}

void loop() {
  int PotValue = analogRead(Pot1); // Read the potentiometer value (0-1023)

  // Debugging: Print the potentiometer value to the serial monitor
  Serial.print("Potentiometer: ");
  Serial.println(PotValue);

  // Convert the potentiometer value into digits and display them
  WriteNumberToSegment(0, PotValue / 1000);         // Thousands place
  WriteNumberToSegment(1, (PotValue / 100) % 10);   // Hundreds place
  WriteNumberToSegment(2, (PotValue / 10) % 10);    // Tens place
  WriteNumberToSegment(3, PotValue % 10);           // Units place

  delay(10); // Small delay to stabilize the display
}