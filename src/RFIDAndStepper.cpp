// #include <SPI.h>
// #include <MFRC522.h>
// #include <Stepper.h>

// #define SPIN4 D4
// #define SPIN3 D3
// #define SPIN2 D2
// #define SPIN1 D1

// int stepsPerRevolultion = 2038;
// Stepper myStepper(stepsPerRevolultion, D4, D3, D2, D1); // Change pins, if needed, (IN1, IN2, IN3, IN4)

// //stepper motor stuff
// void openLock(Stepper step)
//     {
//     step.setSpeed(5);
//     step.step(1019); // Move the stepper motor 1000 steps
// }

// void closeLock(Stepper step)
//     {
//     step.setSpeed(5);
//     step.step(-1019); // Move the stepper motor 1000 steps
// }
// #define SS_PIN D8
// #define RST_PIN D0


// MFRC522 rfid(SS_PIN, RST_PIN);

// int customersInside = 0;

// // in the loop, always check potentiometer value
// // 
// //potentiometer function, that sets the amount of maxCustomers
  
// byte allowedTags[5][4]; // Stores the NUID of the allowed tags

// // finds tag in the allowTags byte array
// bool findTag(byte *uid, int uidSize) {
//   for (int i = 0; i < customersInside; i++) {
//     bool found = true;
//     for (int j = 0; j < uidSize; j++) {
//       if (allowedTags[i][j] != uid[j]) {
//         found = false;
//         break;
//       }
//     }
//     if (found) {
//       return true;
//     }
//   }
//   return false;
// }

// // removes tag when the same chip is read again
// void removeTag(byte *uid, int uidSize) {
//   for (int i = 0; i < customersInside; i++) {
//     bool found = true;
//     for (int j = 0; j < uidSize; j++) {
//       if (allowedTags[i][j] != uid[j]) {
//         found = false;
//         break;
//       }
//     }
//     if (found) {
//       for (int k = i; k < customersInside - 1; k++) {
//         memcpy(allowedTags[k], allowedTags[k + 1], uidSize);
//       }
//       customersInside--;
//       break;
//     }
//   }
// }

// void printNUID(byte *uid, int uidSize) {
//   for (int i = 0; i < uidSize; i++) {
//     if (uid[i] < 0x10) {
//       Serial.print("0");
//     }
//     Serial.print(uid[i], HEX);
//     if (i < uidSize - 1) {
//       Serial.print(":");
//     }
//   }
//   Serial.println();
// }

// void setup() {


//   Serial.begin(9600);
  
//   SPI.begin();
//   rfid.PCD_Init();
// }
// // analog sensor 
// // 
// void loop() {
  
//   int sensorValue = analogRead(A0);
//   byte maxCustomers = map(sensorValue, 0, 1023, 1, 5);
  
  
//   if (!rfid.PICC_IsNewCardPresent()) return;
//   if (!rfid.PICC_ReadCardSerial()) return;

//   // get amount of customers from potentiometer reading
//   Serial.print("RFID NUID: ");
//   printNUID(rfid.uid.uidByte, rfid.uid.size);
//   // if there the chip has already been read, and is then read again, it will remove the tag
//   if (findTag(rfid.uid.uidByte, rfid.uid.size)) {
//     removeTag(rfid.uid.uidByte, rfid.uid.size);
//     Serial.println("Access revoked.");
//     Serial.print("Customers inside: ");
//     Serial.println(customersInside);
//     Serial.println(maxCustomers);

//     // here we will replace the 2 with a 
//   } else if (customersInside < maxCustomers) {
//     memcpy(allowedTags[customersInside], rfid.uid.uidByte, rfid.uid.size);
//     customersInside++;
//     Serial.println("Access granted.");
//     Serial.print("Customers inside: ");
//     Serial.println(customersInside);
//     Serial.println(maxCustomers);
//     openLock(myStepper);
//     closeLock(myStepper);
    
//   } else {
//     Serial.println("Access denied. Maximum number of customers reached.");
//     Serial.println(maxCustomers);
//   }

//   rfid.PICC_HaltA();
//   rfid.PCD_StopCrypto1();
// }