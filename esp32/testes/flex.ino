#include <WiFi.h>
#include <Wire.h>

// Wi-Fi criado pelo ESP32-C3
const char* ssid = "ESP32-C3";
const char* password = "supermini";

WiFiServer server(80);

// Endereço I2C do MPU6050
const int MPU = 0x68;

// Sensores flex
const int FLEX1 = 0;
const int FLEX2 = 1;
const int FLEX3 = 2;


// ==========================================
// LEITURA DO MPU6050
// ==========================================

void lerMPU(
  int16_t &ax,
  int16_t &ay,
  int16_t &az,
  int16_t &gx,
  int16_t &gy,
  int16_t &gz
) {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU, 14);

  if (Wire.available() < 14) {
    ax = ay = az = 0;
    gx = gy = gz = 0;
    return;
  }

  ax = (Wire.read() << 8) | Wire.read();
  ay = (Wire.read() << 8) | Wire.read();
  az = (Wire.read() << 8) | Wire.read();

  // Ignora a temperatura
  Wire.read();
  Wire.read();

  gx = (Wire.read() << 8) | Wire.read();
  gy = (Wire.read() << 8) | Wire.read();
  gz = (Wire.read() << 8) | Wire.read();
}


// ==========================================
// CONFIGURAÇÃO
// ==========================================

void setup() {
  Serial.begin(115200);

  // Sensores flex
  analogReadResolution(12);

  pinMode(FLEX1, INPUT);
  pinMode(FLEX2, INPUT);
  pinMode(FLEX3, INPUT);

  // MPU6050: SDA = GPIO 8, SCL = GPIO 9
  Wire.begin(8, 9);

  // Ativa o MPU6050
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();

  Serial.println("MPU6050 iniciado!");

  // Cria a rede Wi-Fi
  WiFi.softAP(ssid, password);
  WiFi.setTxPower(WIFI_POWER_8_5dBm);

  IPAddress IP = WiFi.softAPIP();

  Serial.print("IP: ");
  Serial.println(IP);

  server.begin();

  Serial.println("Servidor iniciado!");
  Serial.println("Conecte-se ao Wi-Fi ESP32-C3");
}


// ==========================================
// LOOP PRINCIPAL
// ==========================================

void loop() {
  WiFiClient client = server.available();

  if (!client) {
    return;
  }

  Serial.println("Cliente conectado.");

  // Aguarda a requisição
  unsigned long inicio = millis();

  while (!client.available() &&
         millis() - inicio < 100) {
    yield();
  }

  if (!client.available()) {
    client.stop();
    return;
  }

  // Lê a requisição HTTP
  String request = client.readStringUntil('\r');

  Serial.println(request);


  // ========================================
  // ENVIO DOS DADOS DOS SENSORES
  // ========================================

  if (request.indexOf("GET /sensores") >= 0) {
    int16_t ax, ay, az;
    int16_t gx, gy, gz;

    lerMPU(ax, ay, az, gx, gy, gz);

    // Acelerômetro em m/s²
    float acelX = ax * 9.80665 / 16384.0;
    float acelY = ay * 9.80665 / 16384.0;
    float acelZ = az * 9.80665 / 16384.0;

    // Giroscópio em graus por segundo
    float giroX = gx / 131.0;
    float giroY = gy / 131.0;
    float giroZ = gz / 131.0;

    // Leitura dos três sensores flex
    int flex1 = analogRead(FLEX1);
    int flex2 = analogRead(FLEX2);
    int flex3 = analogRead(FLEX3);

    // Cabeçalho HTTP
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Cache-Control: no-cache");
    client.println("Connection: close");
    client.println();

    // Dados do acelerômetro
    client.print("AX:");
    client.print(acelX, 2);

    client.print(" AY:");
    client.print(acelY, 2);

    client.print(" AZ:");
    client.print(acelZ, 2);

    // Dados do giroscópio
    client.print(" GX:");
    client.print(giroX, 2);

    client.print(" GY:");
    client.print(giroY, 2);

    client.print(" GZ:");
    client.print(giroZ, 2);

    // Dados dos sensores flex
    client.print(" FLEX1:");
    client.print(flex1);

    client.print(" FLEX2:");
    client.print(flex2);

    client.print(" FLEX3:");
    client.print(flex3);
  }


  // ========================================
  // PÁGINA PRINCIPAL
  // ========================================

  else {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html; charset=UTF-8");
    client.println("Cache-Control: no-cache");
    client.println("Connection: close");
    client.println();

    client.println("<!DOCTYPE html>");
    client.println("<html lang='pt-BR'>");
    client.println("<head>");

    client.println("<meta charset='UTF-8'>");
    client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    client.println("<title>ESP32-C3 - Sensores</title>");

    // CSS
    client.println("<style>");

    client.println("body {");
    client.println("background: black;");
    client.println("color: lime;");
    client.println("font-family: monospace;");
    client.println("text-align: center;");
    client.println("margin: 20px;");
    client.println("}");

    client.println("h1 { font-size: 28px; }");
    client.println("h2 { font-size: 22px; margin-top: 30px; }");

    client.println(".valor {");
    client.println("font-size: 22px;");
    client.println("margin: 10px;");
    client.println("}");

    client.println("</style>");
    client.println("</head>");

    // Corpo da página
    client.println("<body>");

    client.println("<h1>ESP32-C3</h1>");

    // Acelerômetro
    client.println("<h2>Acelerômetro</h2>");

    client.println("<div class='valor'>X: <span id='ax'>--</span> m/s²</div>");
    client.println("<div class='valor'>Y: <span id='ay'>--</span> m/s²</div>");
    client.println("<div class='valor'>Z: <span id='az'>--</span> m/s²</div>");

    // Giroscópio
    client.println("<h2>Giroscópio</h2>");

    client.println("<div class='valor'>X: <span id='gx'>--</span> °/s</div>");
    client.println("<div class='valor'>Y: <span id='gy'>--</span> °/s</div>");
    client.println("<div class='valor'>Z: <span id='gz'>--</span> °/s</div>");

    // Sensores flex
    client.println("<h2>Sensores Flex</h2>");

    client.println("<div class='valor'>Dedo 1: <span id='flex1'>--</span></div>");
    client.println("<div class='valor'>Dedo 2: <span id='flex2'>--</span></div>");
    client.println("<div class='valor'>Dedo 3: <span id='flex3'>--</span></div>");

    // JavaScript
    client.println("<script>");

    client.println("function atualizarSensores() {");

    client.println("fetch('/sensores?t=' + Date.now())");
    client.println(".then(response => response.text())");
    client.println(".then(dados => {");

    client.println("let valores = dados.trim().split(' ');");

    // Acelerômetro
    client.println("document.getElementById('ax').textContent = valores[0].split(':')[1];");
    client.println("document.getElementById('ay').textContent = valores[1].split(':')[1];");
    client.println("document.getElementById('az').textContent = valores[2].split(':')[1];");

    // Giroscópio
    client.println("document.getElementById('gx').textContent = valores[3].split(':')[1];");
    client.println("document.getElementById('gy').textContent = valores[4].split(':')[1];");
    client.println("document.getElementById('gz').textContent = valores[5].split(':')[1];");

    // Sensores flex
    client.println("document.getElementById('flex1').textContent = valores[6].split(':')[1];");
    client.println("document.getElementById('flex2').textContent = valores[7].split(':')[1];");
    client.println("document.getElementById('flex3').textContent = valores[8].split(':')[1];");

    client.println("})");
    client.println(".catch(erro => console.log(erro));");

    client.println("}");

    // Atualiza a página a cada 100 ms
    client.println("atualizarSensores();");
    client.println("setInterval(atualizarSensores, 100);");

    client.println("</script>");

    client.println("</body>");
    client.println("</html>");
  }

  client.stop();

  Serial.println("Cliente desconectado.");
}
