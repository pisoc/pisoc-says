#define SW_RED 7
#define SW_GREEN 6
#define SW_BLUE 5
#define SW_YELLOW 4
#define SW_START 3

#define LED_RED 11
#define LED_GREEN 10
#define LED_BLUE 9
#define LED_YELLOW 8

void setup() {
  Serial.begin(9600);

  //Get a random seed for the random number generator
  randomSeed(analogRead(0));

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
void initSeq(int seq[], int seqLen) {
  for (int i = 0; i < seqLen; i++) {
    int num = random(0, 4);
    switch (num) {
      case 0:
        seq[i] = LED_RED;
        break;
      case 1:
        seq[i] = LED_GREEN;
        break;
      case 2:
        seq[i] = LED_BLUE;
        break;
      case 3:
        seq[i] = LED_YELLOW;
        break;
    }
  }
  /*
    seq[0] = LED_RED;
    seq[1] = LED_GREEN;
    seq[2] = LED_BLUE;
    seq[3] = LED_YELLOW;
  */
}

// Displays the current sequence to the LEDs
void showSeq(int seq[], int seqLen) {
  for (int i = 0; i < seqLen; i++) {
    digitalWrite(seq[i], HIGH);
    delay(1000);
    digitalWrite(seq[i], LOW);
    delay(200);
  }
}

bool debounce() {
  return digitalRead(SW_RED) || digitalRead(SW_GREEN) || digitalRead(SW_BLUE) || digitalRead(SW_YELLOW);
}

// Read the player's attempt at the game sequence
int readSeq(int seq[], int seqLen) {
  int pos = 0;
  int choice;
  while (pos < seqLen) {
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

      if (digitalRead(SW_START) == HIGH) {
        return 2;
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
  int level = 0;
  const int maxLevel = 3;
  int sequence[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int baseDifficulty = 1;
  const int difficultyIncrease = 1;
  Serial.println("Ready!");
  // Game loop
  while (1) {
    int result;
    if (level == 0) {
      // Wait for start button to be pressed
      while (digitalRead(SW_START) == 0) {}
      // Initialize the sequence
      initSeq(sequence, baseDifficulty);
    } else {
      delay(1000);
      int seq[difficultyIncrease];
      // Get new elements for the sequence
      initSeq(seq, difficultyIncrease);
      // Append new elements to the main sequence
      for (int i = 0; i < difficultyIncrease; i++){
        sequence[baseDifficulty + i] = seq[i];
      }
      baseDifficulty = baseDifficulty + difficultyIncrease;
    }

    int seqLen = baseDifficulty;
    Serial.print("Difficulty: ");
    Serial.println(baseDifficulty);
    Serial.println("Sequence: ");
    for (int x = 0; x< 16; x++){
      Serial.print(sequence[x]);
      Serial.print(", ");
    }
    Serial.println();

    // Display the sequence
    showSeq(sequence, seqLen);

    // Wait for user input
    result = readSeq(sequence, seqLen);

    // Check the result from readSeq
    if (result == 1) {
      if (level == maxLevel) {
        Serial.println("Level cleared!");
        winAnimation();
        level = 0;
        // Start over
        break;
      } else {
        level++;
        // Continue to next level
        continue;
      }
    } else if (result == 0) {
      Serial.println("You lost! :(");
      loseAnimation();
      level = 0;
      // Start over
      break;
    } else if (result == 2) {
      Serial.println("Restarting game...");
      // Start over
      break;
    }
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
