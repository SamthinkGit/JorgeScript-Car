
void setup() {
  Serial.begin(9600);  // Para depuración en monitor serie
}

void loop() {
  // Enviar un mensaje estructurado
  Serial.println("<LINE_FOUND:12>");
  
  delay(1000); // Esperar un segundo
}

