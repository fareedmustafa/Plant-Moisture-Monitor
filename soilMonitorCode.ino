#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <SMTPClient.h>

// Replace with your network credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// SMTP server credentials
const char* smtp_server = "smtp.your-email-provider.com";
const int smtp_port = 587; // Port number
const char* email_user = "your_email@example.com";
const char* email_password = "your_email_password";

// Email details
const char* recipient_email = "recipient_email@example.com";
const char* email_subject = "Soil Moisture Alert";
const char* email_body = "The soil moisture level is low. Please check the plant.";

const int moistureSensorPin = A0;
const int threshold = 1700; // Adjust this value as per your sensor's range

WiFiClientSecure client;
SMTPClient smtpClient;

void setup() {
  Serial.begin(115200);
  
  // Initialize WiFi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  
  // Initialize SMTP client
  smtpClient.setServer(smtp_server, smtp_port);
  smtpClient.setLogin(email_user, email_password);
  
  // Initialize secure connection
  client.setInsecure(); // Use this if you don't have a certificate
}

void loop() {
  int moistureLevel = analogRead(moistureSensorPin);
  
  if (moistureLevel < threshold) {
    sendEmail();
    delay(60000); // Delay to prevent multiple emails, adjust as necessary
  }
  
  delay(10000); // Delay before next reading
}

void sendEmail() {
  smtpClient.setFrom(email_user);
  smtpClient.setTo(recipient_email);
  smtpClient.setSubject(email_subject);
  smtpClient.setMessage(email_body);
  
  if (smtpClient.send()) {
    Serial.println("Email sent successfully");
  } else {
    Serial.println("Failed to send email");
  }
}
