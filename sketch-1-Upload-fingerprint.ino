#include <Adafruit_Fingerprint.h>

// Define constants
#define MAX_USERS 100 // Maximum number of users that can be enrolled

Adafruit_Fingerprint finger(&Serial2); // Use Serial2 for the fingerprint sensor
int userID = 1; // Start with User ID 1

// Define a struct to hold user information
struct User {
  int id;
  String name;
  String department;
};

// Array to store user data
User users[MAX_USERS];
int userCount = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Fingerprint sensor detected!");
  } else {
    Serial.println("Fingerprint sensor not detected. Check connections.");
    while (1);
  }
}

void loop() {
  if (userCount >= MAX_USERS) {
    Serial.println("User database is full!");
    return;
  }

  Serial.print("Enrolling User ID: ");
  Serial.println(userID);

  Serial.println("Enter user name:");
  while (!Serial.available()); // Wait for user input
  String userName = Serial.readStringUntil('\n');
  userName.trim();

  Serial.println("Enter department:");
  while (!Serial.available()); // Wait for user input
  String userDepartment = Serial.readStringUntil('\n');
  userDepartment.trim();

  int result = enrollFingerprint(userID);
  if (result == FINGERPRINT_OK) {
    Serial.println("User enrolled successfully!");
    // Store user information
    users[userCount].id = userID;
    users[userCount].name = userName;
    users[userCount].department = userDepartment;
    userCount++;
    userID++; // Increment User ID for the next user

    // Print stored information
    Serial.println("Current Users:");
    for (int i = 0; i < userCount; i++) {
      Serial.print("ID: ");
      Serial.print(users[i].id);
      Serial.print(", Name: ");
      Serial.print(users[i].name);
      Serial.print(", Department: ");
      Serial.println(users[i].department);
    }
  } else {
    Serial.println("Enrollment failed. Try again.");
  }
  delay(3000);
}

int enrollFingerprint(int id) {
  Serial.println("Place your finger on the sensor...");
  while (finger.getImage() != FINGERPRINT_OK);

  if (finger.image2Tz(1) != FINGERPRINT_OK) return -1;

  Serial.println("Remove your finger...");
  delay(2000);

  Serial.println("Place the same finger again...");
  while (finger.getImage() != FINGERPRINT_OK);

  if (finger.image2Tz(2) != FINGERPRINT_OK) return -1;

  if (finger.createModel() != FINGERPRINT_OK) return -1;

  if (finger.storeModel(id) == FINGERPRINT_OK) return FINGERPRINT_OK;
  return -1;
}
