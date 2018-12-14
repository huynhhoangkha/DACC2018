#include "sensor.h"
#include "InOut.h"
#include <SoftwareSerial.h>
#include <lcd.h>

Adafruit_Fingerprint finger = Adafruit_Fingerprint(new SoftwareSerial(FP_TX, FP_RX));

void connectSensor() {
	Serial.println("\n\nConnecting to the fingerprint sensor...");

	// set the data rate for the sensor serial port
	finger.begin(SENSOR_BAUDRATE);
	clearLCD();
	display("Finger sensor", 0, 0);
	if (finger.verifyPassword()) {
		Serial.println("Found fingerprint sensor!");
		display("Verify OK.", 0, 1);
	}
	else {
		Serial.println("Did not find fingerprint sensor :(");
		display("Verify failed.", 0, 1);
		return;
	}

	finger.getTemplateCount();
	if (finger.templateCount == 0) {
		Serial.println("No fingerprint found, please enroll");
		while (getFingerprintEnroll(0) == -1) delay(10);
	}
	else {
		Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
		Serial.println("Waiting for valid finger...");
	}
}

bool getFingerprintIDez(int& id) {
	uint8_t p = finger.getImage();
	if (p != FINGERPRINT_OK)  return false;
	beep(200);
	p = finger.image2Tz();
	if (p != FINGERPRINT_OK)  return false;

	p = finger.fingerFastSearch();
	if (p != FINGERPRINT_OK)  return false;

	// found a match!
	Serial.print("Found ID #"); Serial.print(finger.fingerID);
	Serial.print(" with confidence of "); Serial.println(finger.confidence);
	id = finger.fingerID;
	return true;
}

bool getFingerprintID(int& id) {
	uint8_t p = finger.getImage();
	switch (p) {
	case FINGERPRINT_OK:
		beep(200);
		break;
	case FINGERPRINT_NOFINGER:
		id = -1;
		return false;
	case FINGERPRINT_PACKETRECIEVEERR:
		Serial.println("Communication error");
		return false;
	case FINGERPRINT_IMAGEFAIL:
		Serial.println("Imaging error");
		return false;
	default:
		Serial.println("Unknown error");
		return false;
	}

	// OK success!

	p = finger.image2Tz();
	switch (p) {
	case FINGERPRINT_OK:
		Serial.println("Image converted");
		break;
	case FINGERPRINT_IMAGEMESS:
		Serial.println("Image too messy");
		return false;
	case FINGERPRINT_PACKETRECIEVEERR:
		Serial.println("Communication error");
		return false;
	case FINGERPRINT_FEATUREFAIL:
		Serial.println("Could not find fingerprint features");
		return false;
	case FINGERPRINT_INVALIDIMAGE:
		Serial.println("Could not find fingerprint features");
		return false;
	default:
		Serial.println("Unknown error");
		return false;
	}

	// OK converted!
	p = finger.fingerFastSearch();
	if (p == FINGERPRINT_OK) {
		Serial.println("Found a print match!");
	}
	else if (p == FINGERPRINT_PACKETRECIEVEERR) {
		Serial.println("Communication error");
		return false;
	}
	else if (p == FINGERPRINT_NOTFOUND) {
		Serial.println("Did not find a match");
		id = -2;
		return false;
	}
	else {
		Serial.println("Unknown error");
		return false;
	}

	// found a match!
	Serial.print("Found ID #"); Serial.print(finger.fingerID);
	Serial.print(" with confidence of "); Serial.println(finger.confidence);
	id = finger.fingerID;
	return true;
}

int getFingerprintEnroll(uint8_t id) {
	int p = -1;
	clearLCD();
	display("Touch the sensor", 0, 0);
	display("to enroll finger", 0, 1);
	long t = millis();
	while (p != FINGERPRINT_OK && millis() - t < 5000) {
		p = finger.getImage();
		switch (p) {
		case FINGERPRINT_OK:
			beep(200);
			Serial.println("Image taken");
			break;
		case FINGERPRINT_NOFINGER:
			Serial.print(".");
			break;
		case FINGERPRINT_PACKETRECIEVEERR:
			Serial.println("Communication error");
			break;
		case FINGERPRINT_IMAGEFAIL:
			Serial.println("Imaging error");
			break;
		default:
			Serial.println("Unknown error");
			break;
		}
	}
	if (p != FINGERPRINT_OK) {
		Serial.println("Timed out.");
		return -1;
	}
	// OK success!

	p = finger.image2Tz(1);
	switch (p) {
	case FINGERPRINT_OK:
		Serial.println("Image converted");
		break;
	case FINGERPRINT_IMAGEMESS:
		Serial.println("Image too messy");
		return -1;
	case FINGERPRINT_PACKETRECIEVEERR:
		Serial.println("Communication error");
		return -1;
	case FINGERPRINT_FEATUREFAIL:
		Serial.println("Could not find fingerprint features");
		return -1;
	case FINGERPRINT_INVALIDIMAGE:
		Serial.println("Could not find fingerprint features");
		return -1;
	default:
		Serial.println("Unknown error");
		return -1;
	}

	//Serial.println("Remove finger");
	//delay(2000);

	p = 0;
	while (p != FINGERPRINT_NOFINGER) {
		p = finger.getImage();
	}
	Serial.print("ID "); Serial.println(id);
	p = -1;
	Serial.println("Place same finger again");
	clearLCD();
	display("Touch the sensor", 0, 0);
	display("again to finish.", 0, 1);
	t = millis();
	while (p != FINGERPRINT_OK && millis() - t < 5000) {
		p = finger.getImage();
		switch (p) {
		case FINGERPRINT_OK:
			beep(200);
			Serial.println("Image taken");
			break;
		case FINGERPRINT_NOFINGER:
			Serial.print(".");
			break;
		case FINGERPRINT_PACKETRECIEVEERR:
			Serial.println("Communication error");
			break;
		case FINGERPRINT_IMAGEFAIL:
			Serial.println("Imaging error");
			break;
		default:
			Serial.println("Unknown error");
			break;
		}
	}
	if (p != FINGERPRINT_OK) {
		Serial.println("Timed out.");
		return -1;
	}
	// OK success!

	p = finger.image2Tz(2);
	switch (p) {
	case FINGERPRINT_OK:
		Serial.println("Image converted");
		break;
	case FINGERPRINT_IMAGEMESS:
		Serial.println("Image too messy");
		return -1;
	case FINGERPRINT_PACKETRECIEVEERR:
		Serial.println("Communication error");
		return -1;
	case FINGERPRINT_FEATUREFAIL:
		Serial.println("Could not find fingerprint features");
		return -1;
	case FINGERPRINT_INVALIDIMAGE:
		Serial.println("Could not find fingerprint features");
		return -1;
	default:
		Serial.println("Unknown error");
		return -1;
	}

	// OK converted!
	Serial.print("Creating model for #");  Serial.println(id);

	p = finger.createModel();
	if (p == FINGERPRINT_OK) {
		Serial.println("Prints matched!");
	}
	else if (p == FINGERPRINT_PACKETRECIEVEERR) {
		Serial.println("Communication error");
		return -1;
	}
	else if (p == FINGERPRINT_ENROLLMISMATCH) {
		Serial.println("Fingerprints did not match");
		return -1;
	}
	else {
		Serial.println("Unknown error");
		return -1;
	}

	Serial.print("ID "); Serial.println(id);
	p = finger.storeModel(id);
	while (p != FINGERPRINT_OK) {
		if (p == FINGERPRINT_PACKETRECIEVEERR) {
			Serial.println("Communication error");
			return -1;
		}
		else if (p == FINGERPRINT_BADLOCATION) {
			Serial.print("Could not store as #");
			Serial.println(id);
			p = finger.storeModel(++id);
			if (p == FINGERPRINT_OK) {
				Serial.print("Stored new finger print as #");
				Serial.println(id);
				clearLCD();
				display("Added.", 0, 0);
				return id;
			}
			else continue;
		}
		else if (p == FINGERPRINT_FLASHERR) {
			Serial.println("Error writing to flash");
			return -1;
		}
		else {
			Serial.println("Unknown error");
			return -1;
		}
	}
	state = _INIT;
}

uint8_t deleteFingerprint(uint8_t id) {
	uint8_t p = -1;

	p = finger.deleteModel(id);

	if (p == FINGERPRINT_OK) {
		Serial.println("Deleted!");
	}
	else if (p == FINGERPRINT_PACKETRECIEVEERR) {
		Serial.println("Communication error");
		return p;
	}
	else if (p == FINGERPRINT_BADLOCATION) {
		Serial.println("Could not delete in that location");
		return p;
	}
	else if (p == FINGERPRINT_FLASHERR) {
		Serial.println("Error writing to flash");
		return p;
	}
	else {
		Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
		return p;
	}
}
