String direction(String raw) {
	return raw.charAt(0);
}

int Speed(String raw) {
	String string = String(raw.charAt(1) + raw.charAt(2) + raw.charAt(3));
	return string.toInt();
}

int Wheel(String raw) {
	String string = String(raw.charAt(4) + raw.charAt(5) + raw.charAt(6));
	return string.toInt();
}

// INITIALIZE RAW DATA
String raw = ""
if (Serial.available()) {
	raw = Serial.readString();
}

