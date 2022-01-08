const byte DISTANCE_QUERY_PIN = 2;
const byte DISTANCE_RESULT_PIN = 3;
const byte RELAY_PIN = 4;
const byte DIODE_PIN = LED_BUILTIN;

void setup()
{
  pinMode(DISTANCE_QUERY_PIN, OUTPUT);
  digitalWrite(DISTANCE_QUERY_PIN, LOW);

  pinMode(DISTANCE_RESULT_PIN, INPUT);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  pinMode(DIODE_PIN, OUTPUT);
  digitalWrite(DIODE_PIN, LOW);
}

void send_distance_query()
{
  digitalWrite(DISTANCE_QUERY_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(DISTANCE_QUERY_PIN, LOW);
}

int read_distance_response()
{
  static const unsigned int SPEED_OF_SOUND = 343; // meters/second
  static const unsigned int MAX_DISTANCE = 4; // meters
  static const unsigned int MAX_RESPONSE_DELAY = (1e6 * MAX_DISTANCE * 2) / SPEED_OF_SOUND; // microseconds
  unsigned long response_delay = pulseIn(DISTANCE_RESULT_PIN, HIGH, MAX_RESPONSE_DELAY); // microseconds
  if (response_delay == 0) {
    return -1;
  }

  // delay -> centimeters:
  // (((response_delay / 1 000 000) * SPEED_OF_SOUND) / 2) * 100
  //   -> response_delay * SPEED_OF_SOUND / 20000
  //   -> response_delay * SPEED_OF_SOUND / 20000
  //   -> response_delay * 0.01715
  return response_delay * 0.01715; // centimeters
}

void loop()
{
  delay(millis() % 1000); // Run once every second
  
  send_distance_query();
  unsigned long distance = read_distance_response();

  static unsigned long last_presence = 0;
  if (distance > 0 && distance < 100) {
    last_presence = millis();
    digitalWrite(DIODE_PIN, HIGH);
    digitalWrite(RELAY_PIN, HIGH);
  }

  // Switch off LED when there hasn't been any presence recently
  if (millis() - last_presence > 10000) {
    digitalWrite(DIODE_PIN, LOW);
    digitalWrite(RELAY_PIN, LOW);
  }
}
