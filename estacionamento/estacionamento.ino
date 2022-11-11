 #include <LiquidCrystal.h>   
   
#define SONAR_NUM   2 // Define o numero de sensores  
 
#define PING_INTERVAL 33 
   
unsigned long pingTimer[SONAR_NUM]; 
unsigned int cm[SONAR_NUM];     // Armazena o numero de medicoes  
uint8_t currentSensor = 0;     // Armazena o sensor que esta ativo  
   
int Pinoled1Verm = A2;    
int Pinoled1Verde = A1;  
int Pinoled2Verm = 8;   
int Pinoled2Verde = 9;  
int vagaslivres = 2;   
int vagasocupadas = 0;   
int sensor1 = 0;    //Contador de vagas no sensor1  
int sensor2 = 0;     //Contador de vagas no sensor2  
int distancia1 = 0;   
int distancia2 = 0;      
//Define os pinos que serao ligados ao LCD  
LiquidCrystal lcd(12,13,7, 6, 5, 4); 
   
     
      
   
void setup()   
{  
  Serial.begin(9600);  
  lcd.begin(16,2); //Inicializa LCD  
  lcd.clear();   //Limpa o LCD  
  pingTimer[0] = millis() + 75;      //Primeira medicao começa com 75ms  
  //Define o tempo de inicializacao de cada sensor
  for (uint8_t i = 1; i < SONAR_NUM; i++)   
   pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;  
  pinMode(Pinoled1Verm, OUTPUT);  
  pinMode(Pinoled1Verde, OUTPUT); 
  pinMode(Pinoled2Verm, OUTPUT);  
  pinMode(Pinoled2Verde, OUTPUT); 
 }  
   
void loop() 
{  

   
  // Loop entre todos os sensores 
  for (uint8_t i = 0; i < SONAR_NUM; i++) {  
   if (millis() >= pingTimer[i]) {

    //Define o tempo que o proximo sensor sera acionado
    pingTimer[i] += PING_INTERVAL * SONAR_NUM;   
    // Ciclo do sensor completo  
    if (i == 0 && currentSensor == SONAR_NUM - 1) oneSensorCycle(); 
    // Reseta o timer antes de ler o proximo sensor  
    // Número do sensor sendo acionado
    currentSensor = i;               
       
    cm[currentSensor] = 0;           
   }  
 }  
    
 
 vagasocupadas = sensor1 + sensor2;  
 vagaslivres = 2 - vagasocupadas;  
 lcd.setCursor(0,0);  
 lcd.print("Vagas livres = ");  
 lcd.print(vagaslivres);  
 lcd.setCursor(0,1);  
 lcd.print("Vagas ocupad = ");  
 lcd.print(vagasocupadas);  
}  
 
void oneSensorCycle() { // Ciclo de leitura do sensor 
distancia1 = 0.01723 * readUltrasonicDistance(11, 10);
  Serial.print(" cm - ");   
  Serial.print(distancia1);
  distancia2 = 0.01723 * readUltrasonicDistance(3, 2);
  Serial.print(" cm - ");   
  Serial.print(distancia2);
  for (uint8_t i = 0; i < SONAR_NUM; i++) {  
   
   if (distancia1 > 1 && distancia1 < 120)   
   {  
     digitalWrite(Pinoled1Verm, 1);  
     digitalWrite(Pinoled1Verde, 0);  
     sensor1 = 1;  
   }  
   else 
   {  
     digitalWrite(Pinoled1Verm, 0);  
     digitalWrite(Pinoled1Verde, 1);  
     sensor1 = 0; 
   }   
   
   
   if (distancia2 > 1 && distancia2 < 120) 
   {  
     digitalWrite(Pinoled2Verm, 1);  
     digitalWrite(Pinoled2Verde, 0);  
     sensor2 = 1; 
   }  
   else 
   {  
     digitalWrite(Pinoled2Verm, 0);  
     digitalWrite(Pinoled2Verde, 1);  
     sensor2 = 0; 
   }   
  
  }  
  Serial.println();  
}
      
      
      
long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  return pulseIn(echoPin, HIGH);
}