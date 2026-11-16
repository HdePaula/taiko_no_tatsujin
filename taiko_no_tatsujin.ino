const int pinoPiezo1 = A0;
const int pinoPiezo2 = A1;
const int pinoPiezo3 = A2;
const int pinoPiezo4 = A4;

// Aumente este valor se a vibração da mesa estiver disparando os pads
const int limiteBatida = 40; 

void setup() {
  Serial.begin(9600);
  Serial.println("--- MODO DE CALIBRACAO ---");
}

// Função especial que limpa o canal analógico do Arduino antes de ler
int lerPiezoLimpo(int pino) {
  analogRead(pino); // Primeira leitura descarta a carga residual do pino anterior
  delayMicroseconds(50);
  return analogRead(pino); // Segunda leitura pega o valor real
}

void loop() {
  int l1 = lerPiezoLimpo(pinoPiezo1);
  int l2 = lerPiezoLimpo(pinoPiezo2);
  int l3 = lerPiezoLimpo(pinoPiezo3);
  int l4 = lerPiezoLimpo(pinoPiezo4);

  // Se qualquer sensor ultrapassar o limite
  if (l1 > limiteBatida || l2 > limiteBatida || l3 > limiteBatida || l4 > limiteBatida) {
    
    int max1 = l1, max2 = l2, max3 = l3, max4 = l4;

    // Monitora durante 8 milissegundos para pegar o pico REAL de cada sensor
    unsigned long inicio = millis();
    while (millis() - inicio < 8) {
      int v1 = lerPiezoLimpo(pinoPiezo1);
      int v2 = lerPiezoLimpo(pinoPiezo2);
      int v3 = lerPiezoLimpo(pinoPiezo3);
      int v4 = lerPiezoLimpo(pinoPiezo4);

      if (v1 > max1) max1 = v1;
      if (v2 > max2) max2 = v2;
      if (v3 > max3) max3 = v3;
      if (v4 > max4) max4 = v4;
    }

    // Mostra qual venceu e exibe o valor dos 4 para você calibrar
    if (max1 >= max2 && max1 >= max3 && max1 >= max4) {
      Serial.print("VENCEDOR: [Piezo 1]  -->  ");
    } 
    else if (max2 >= max1 && max2 >= max3 && max2 >= max4) {
      Serial.print("VENCEDOR: [Piezo 2]  -->  ");
    } 
    else if (max3 >= max1 && max3 >= max2 && max3 >= max4) {
      Serial.print("VENCEDOR: [Piezo 3]  -->  ");
    } 
    else {
      Serial.print("VENCEDOR: [Piezo 4]  -->  ");
    }

    // Imprime os valores individuais para diagnóstico
    Serial.print("P1: "); Serial.print(max1);
    Serial.print(" | P2: "); Serial.print(max2);
    Serial.print(" | P3: "); Serial.print(max3);
    Serial.print(" | P4: "); Serial.println(max4);

    // Aguarda a mesa parar de vibrar
    delay(150);
    while (lerPiezoLimpo(pinoPiezo1) > limiteBatida || 
           lerPiezoLimpo(pinoPiezo2) > limiteBatida || 
           lerPiezoLimpo(pinoPiezo3) > limiteBatida || 
           lerPiezoLimpo(pinoPiezo4) > limiteBatida) {
      delay(10);
    }
  }
}
