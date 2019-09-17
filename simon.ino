#define SW_RED 7
#define SW_GREEN 6
#define SW_BLUE 5
#define SW_YELLOW 4
#define SW_START 3

#define LED_RED 12
#define LED_GREEN 11
#define LED_BLUE 10
#define LED_YELLOW 9

int level = 1;

void setup() {
  Serial.begin(9600);

  pinMode(SW_RED, INPUT);
  pinMode(SW_GREEN, INPUT);
  pinMode(SW_BLUE, INPUT);
  pinMode(SW_YELLOW, INPUT);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);

  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_YELLOW, LOW);
}

// Generate initial game sequence
void initSeq(int seq[]) {
  seq[0] = LED_RED;
  seq[1] = LED_GREEN;
  seq[2] = LED_BLUE;
  seq[3] = LED_YELLOW;
}

// Displays the current sequence to the LEDs
void showSeq(int seq[]) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(seq[i], HIGH);
    delay(1000);
    digitalWrite(seq[i], LOW);
  }
}

bool debounce() {
  return digitalRead(SW_RED) || digitalRead(SW_GREEN) || digitalRead(SW_BLUE) || digitalRead(SW_YELLOW);
}

// Read the player's attempt at the game sequence
int readSeq(int seq[]) {
  int pos = 0;
  int choice;
  while (pos < 4) {
    Serial.println("Starting read loop");
    choice = 0;

    // Read one choice from player
    while (choice == 0) {
      if (digitalRead(SW_RED) == HIGH) {
        digitalWrite(LED_RED, HIGH);
        choice = LED_RED;
        Serial.println("Got input from red");
        break;
      }

      if (digitalRead(SW_GREEN) == HIGH) {
        digitalWrite(LED_GREEN, HIGH);
        choice = LED_GREEN;
        Serial.println("Got input from green");
        break;
      }

      if (digitalRead(SW_BLUE) == HIGH) {
        digitalWrite(LED_BLUE, HIGH);
        choice = LED_BLUE;
        Serial.println("Got input from blue");
        break;
      }

      if (digitalRead(SW_YELLOW) == HIGH) {
        digitalWrite(LED_YELLOW, HIGH);
        choice = LED_YELLOW;
        Serial.println("Got input from yellow");
        break;
      }
    }


    Serial.println("Read choice");
    Serial.print("Selected: "),
                 Serial.println(choice);
    Serial.print("Expected: ");
    Serial.println(seq[pos]);

    // Wait for button do debounce
    while (debounce()) {}

    // Compare choice to sequence
    if (seq[pos] != choice) {
      Serial.println("Wrong choice!");
      return 0;
    }

    pos++;
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_YELLOW, LOW);
  }
  return 1;
}

void printArray(int arr[]) {
  for (int i = 0; i < 4; i++) {
    Serial.println(arr[i]);
  }
}

// the loop function runs over and over again forever
void loop() {
  Serial.println("Ready!");

  while (digitalRead(SW_START) == 0) {}

  int seq[4];
  int result;

  initSeq(seq);

  showSeq(seq);

  result = readSeq(seq);

  if (result) {
    Serial.println("You won!");
    winAnimation();
  } else {
    Serial.println("You lost! :(");
    loseAnimation();
  }
}

void loseAnimation() {

  int colours[4];
  colours[0] = LED_RED;
  colours[1] = LED_GREEN;
  colours[2] = LED_BLUE;
  colours[3] = LED_YELLOW;

  int sequence[8][4] = {
    {0, 0, 0, 0},
    {0, 1, 0, 1},
    {1, 0, 1, 0},
    {0, 1, 0, 1},
    {1, 0, 1, 0},
    {0, 1, 0, 1},
    {1, 0, 1, 0},
    {0, 1, 0, 1}
  };
  for (int i = 0; i < (sizeof(sequence) / sizeof(sequence[0])); i++) {
    for (int j = 0; j < 4; j++) {
      digitalWrite(colours[j], sequence[i][j]);
    }
    delay(1000);
  }

  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_YELLOW, LOW);
}

void winAnimation() {

  int colours[4];
  colours[0] = LED_RED;
  colours[1] = LED_GREEN;
  colours[2] = LED_BLUE;
  colours[3] = LED_YELLOW;

  int sequence[18][4] = {
    {0, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 1},
    {1, 0, 0, 1},
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 1},
    {1, 0, 0, 1},
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 1},
    {1, 0, 0, 1},
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 1},
    {1, 0, 0, 1},
    {1, 1, 0, 0}
  };
  for (int i = 0; i < (sizeof(sequence) / sizeof(sequence[0])); i++) {
    for (int j = 0; j < 4; j++) {
      digitalWrite(colours[j], sequence[i][j]);
    }
    delay(200);
  }

  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_YELLOW, LOW);
}
