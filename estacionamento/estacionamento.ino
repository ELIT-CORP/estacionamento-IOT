 #include <LiquidCrystal.h>   
   
#define SONAR_NUM   2 // Define o numero de sensores  
// Milisegundos de intervalo entre medicoes (29ms e o tempo mínimo para 
// evitar conflito entre os sensores)  
#define PING_INTERVAL 33 
   
// Armazena a quantidade de vezes que a medicao deve ocorrer,para cada sensor  
unsigned long pingTimer[SONAR_NUM]; 
unsigned int cm[SONAR_NUM];     // Armazena o numero de medicoes  
uint8_t currentSensor = 0;     // Armazena o sensor que esta ativo  
   
int Pinoled1Verm = A2;  //Pino led1 - Vermelho  
int Pinoled1Verde = A1; //Pino led1 - Verde  
int Pinoled2Verm = 8;  //Pino led2 - Vermelho  
int Pinoled2Verde = 9; //Pino led2 - Verde  
int vagaslivres = 2;   //Contador de vagas livres  
int vagasocupadas = 0;  //Contador de vagas ocupadas  
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
  pinMode(Pinoled1Verm, OUTPUT);  //Define o Pino vermelho do led1 como saida  
  pinMode(Pinoled1Verde, OUTPUT); //Define o Pino verde do led1 como saida  
  pinMode(Pinoled2Verm, OUTPUT);  //Define o Pino vermelho do led2 como saida  
  pinMode(Pinoled2Verde, OUTPUT); //Define o Pino verde do led2 como saida  
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
    // Se nao houver eco do sensor, seta a distância como zero   
    cm[currentSensor] = 0;           
   }  
 }  
    
 //Calcula a quantidade de vagas disponiveis e ocupadas, e imprime no display  
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
   //Se for detectado objeto entre 0 e 50 cm do sensor1, acende o led1 vermelho
   if (distancia1 > 1 && distancia1 < 120)   
   {  
     digitalWrite(Pinoled1Verm, 1);  
     digitalWrite(Pinoled1Verde, 0);  
     sensor1 = 1; //Incrementa o número de vagas ocupadas na vaga1  
   }  
   else //Se não for detectado objeto no sensor 1, mantém o led1 verde aceso  
   {  
     digitalWrite(Pinoled1Verm, 0);  
     digitalWrite(Pinoled1Verde, 1);  
     sensor1 = 0; //Marca a vaga 1 como livre  
   }   
   
   //Se for detectado objeto entre 0 e 50 cm do sensor2, acende o led2 vermelho  
   if (distancia2 > 1 && distancia2 < 120) 
   {  
     digitalWrite(Pinoled2Verm, 1);  
     digitalWrite(Pinoled2Verde, 0);  
     sensor2 = 1; //Incrementa o número de vagas ocupadas na vaga2  
   }  
   else //Se não for detectado objeto no sensor 2, mantém o led2 verde aceso  
   {  
     digitalWrite(Pinoled2Verm, 0);  
     digitalWrite(Pinoled2Verde, 1);  
     sensor2 = 0; //Marca a vaga 2 como livre  
   }   
  
  }  
  Serial.println();  
}
      
      
      
long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}