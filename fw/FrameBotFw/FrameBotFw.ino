/*
  FrameBot controll

  Data: 19/08/2016
  
  Ver: 1.0

  Autor: Matteo Saccaro
  
  Note:
       - This program started by Web Server example on Fishino Board (http://www.fishino.it)
       - Compile this program with Arduino UNO board          
       - configure the wifi.
				 Comment:
       			#define STANDALONE_MODE
       	 If you want connect at own router. Not comment if you want use in stand alone mode (AP).
       	 And:
       	 			#define MY_SSID
       	 			#define MY_PASS
       	 and eventually
       	 			#define IPADDR
       	 With your personal setting.
       	 
       - The Program, wait a connect from client, send HTML page and wait receive command from client.
       - When command is received then its is parser and action is extract.
       - remove the serial console to save space.
       			#define EN_CONSOLE    0
       		
				 ATTENTION: if serial console enabled in stand alone mode ( no pc connected)
				            remove "while" instruction after init.

   TODO:
       - insert measure battery voltage and if low voltage detected set led.
       - insert watchdog. If web server is broken restart the system.
       - insert management servo motor via define. this is importa to use unique program.
               
 */

#include <Flash.h>
#include <FishinoUdp.h>
#include <FishinoSockBuf.h>
#include <Fishino.h>
#include <SPI.h>

#include <SD.h>

////// framebot appication

#include <Servo.h>


#define MOT1_EN1     14
#define MOT1_EN2     15

#define MOT2_EN1     16
#define MOT2_EN2     17

#define SRV_DX       9
#define SRV_SX       8


#define POSITION_DEFAULT  90
#define POSITION_MIN_BR   60
#define POSITION_MAX     180
#define POSITION_MIN       0

#define EN_CONSOLE    0   /* */
#define MY_WEB_APP 1
#if MY_WEB_APP
Servo servoDx, servoSx;
uint8_t posServoDx,posServoSx;
uint8_t modo_motori, modo_servi;

#define MODO_CONTINUO 0
#define MODO_IMPULSO  1
#define MODO_COLPO    0


#endif
////////// end framebot application


///////////////////////////////////////////////////////////////////////
//           CONFIGURATION DATA -- ADAPT TO YOUR NETWORK             //
//     CONFIGURAZIONE SKETCH -- ADATTARE ALLA PROPRIA RETE WiFi      //

// OPERATION MODE :
// NORMAL (STATION)	-- NEEDS AN ACCESS POINT/ROUTER
// STANDALONE (AP)	-- BUILDS THE WIFI INFRASTRUCTURE ON FISHINO
// COMMENT OR UNCOMMENT FOLLOWING #define DEPENDING ON MODE YOU WANT
// MODO DI OPERAZIONE :
// NORMAL (STATION)	-- HA BISOGNO DI UNA RETE WIFI ESISTENTE A CUI CONNETTERSI
// STANDALONE (AP)	-- REALIZZA UNA RETE WIFI SUL FISHINO
// COMMENTARE O DE-COMMENTARE LA #define SEGUENTE A SECONDA DELLA MODALITÀ RICHIESTA
#define STANDALONE_MODE

// here pur SSID of your network
// inserire qui lo SSID della rete WiFi
#define MY_SSID	"FRAMEBOT"

// here put PASSWORD of your network. Use "" if none
// inserire qui la PASSWORD della rete WiFi -- Usare "" se la rete non ￨ protetta
#define MY_PASS	""


// comment this line if you want a dynamic IP through DHCP
// obtained IP will be printed on serial port monitor
// commentare la linea seguente per avere un IP dinamico tramite DHCP
// l'IP ottenuto verrà visualizzato sul monitor seriale
#define IPADDR 192,168,1,251

//                    END OF CONFIGURATION DATA                      //
//                       FINE CONFIGURAZIONE                         //
///////////////////////////////////////////////////////////////////////

// define ip address if required
#ifdef IPADDR
IPAddress ip(IPADDR);
#endif

FishinoServer server(80);

void printWifiStatus()
{
	// print the SSID of the network you're attached to:
	// stampa lo SSDI della rete a cui si è connessi
	Serial.print("SSID: ");
	Serial.println(Fishino.SSID());

	// get phy mode and show it
	uint8_t mode = Fishino.getPhyMode();
	Serial.print("PHY MODE: (");
	Serial.print(mode);
	Serial.print(") ");
	switch(mode)
	{
		case PHY_MODE_11B:
			Serial.println("11B");
			break;

		case PHY_MODE_11G:
			Serial.println("11G");
			break;

		case PHY_MODE_11N:
			Serial.println("11N");
			break;
			
		default:
			Serial.println("UNKNOWN");
	}
	

	// get AP IP info
	IPAddress ip, gw, nm;
	if(Fishino.getApIPInfo(ip, gw, nm))
	{
		Serial << F("Fishino IP      :") << ip << "\r\n";
		Serial << F("Fishino GATEWAY :") << gw << "\r\n";
		Serial << F("Fishino NETMASK :") << nm << "\r\n";
	}
	else
		Serial << F("Couldn't get Fishino IP info\r\n");

}


void setup()
{
	//Initialize serial and wait for port to open:
	// apre la porta seriale e ne attende l'apertura
#if EN_CONSOLE	
	Serial.begin(115200);
#endif

	// wait for serial port to connect. Needed for Leonardo only
	// attende l'apertura della porta seriale. Necessario solo per le boards Leonardo
#if EN_CONSOLE  
	while (!Serial)
		;
#endif

#if MY_WEB_APP
	//// framebot init
	setupRobot();
#endif

  delay(4000);

	// initialize SPI
	// inizializza il modulo SPI
	SPI.begin();
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	
	// reset and test wifi module
	// resetta e testa il modulo WiFi
#if EN_CONSOLE  	
	Serial << F("Resetting Fishino...");
#endif
	while(!Fishino.reset())
	{
  #if EN_CONSOLE  
		Serial << ".";
  #endif
		delay(500);
	}
#if EN_CONSOLE  	
	Serial << F("OK\r\n");
  Serial << F("Fishino WiFi web server\r\n");
#endif

	

	// set PHY mode to 11G
	Fishino.setPhyMode(PHY_MODE_11G);
	
	// for AP MODE, setup the AP parameters
	// setup SOFT AP mode
	// imposta la modalitè SOFTAP
#if EN_CONSOLE	
	Serial << F("Setting mode SOFTAP_MODE\r\n");
#endif
	Fishino.setMode(SOFTAP_MODE);

	// stop AP DHCP server
#if EN_CONSOLE
	Serial << F("Stopping DHCP server\r\n");
#endif
	Fishino.softApStopDHCPServer();
	
	// setup access point parameters
	// imposta i parametri dell'access point
#if EN_CONSOLE
	Serial << F("Setting AP IP info\r\n");
#endif
	Fishino.setApIPInfo(ip, ip, IPAddress(255, 255, 255, 0));

#if EN_CONSOLE
	Serial << F("Setting AP WiFi parameters\r\n");
#endif
	Fishino.softApConfig(MY_SSID, MY_PASS, 1, false);
	
	// restart DHCP server
#if EN_CONSOLE  
	Serial << F("Starting DHCP server\r\n");
#endif
	Fishino.softApStartDHCPServer();
	

	// show connection status
	// visualizza lo stato della connessione
#if EN_CONSOLE
	printWifiStatus();
#endif

	// start listening for clients
	// inizia l'attesa delle connessioni
	server.begin();

}

#if MY_WEB_APP
uint8_t Comando = 0;
#endif
int MST_IdxExtractCMD;
String CmdRx;


void loop()
{

	// wait for a new client:
	// attende nuovi clienti
	FishinoClient client = server.available();

	if (client)
	{
		//Serial.println("new client");
		
		// an http request ends with a blank line
		// ogni richiesta http termina con una linea vuota
		boolean currentLineIsBlank = true;
		//String CmdRx = "";
		//int caratteri = 0;
    MST_IdxExtractCMD = 0;
    CmdRx = "";

		while (client.connected())
		{
			if (client.available())
			{
				char c = client.read();
			#if EN_CONSOLE	
				Serial.write(c);
      #endif
      
        //CmdRx += c;
        //caratteri++;
        EstraiGET(c);

				// if you've gotten to the end of the line (received a newline
				// character) and the line is blank, the http request has ended,
				// so you can send a reply
				// se si è arrivati a fine linea (carattere 'newline' ricevuto
				// e la linea è vuota, la richiesa http è terminata
				// quindi è possibile inviera una risposta
				if (c == '\n' && currentLineIsBlank)
				{
					// send a standard http response header
					// invia uno header standard http

        #if EN_CONSOLE
          //Serial.print("caratteri: ");
          //Serial.println(caratteri);
          Serial.print("Stringa comando: ");
          Serial.println(CmdRx);
        #endif
    #if 0
          stampaSito(client);
            Comando++;
            if (Comando > 2)
               Comando = 1;
          
    #else
          
					///  GET /btn=up? HTTP/1.1
            if ((CmdRx[0] == 'u') && (CmdRx[1] == 'p')){
             //CmdRx ="";
             Comando  = 1;
            } else if ((CmdRx[0] == 'd') && (CmdRx[1] == 'o') && (CmdRx[2] == 'w') && (CmdRx[3] == 'n')){
             //CmdRx ="";
             Comando  = 2;
            } else if ((CmdRx[0] == 'd') && (CmdRx[1] == 'x') && (CmdRx[2] == 'd')){
             //CmdRx ="";
             Comando  = 6;
            } else if ((CmdRx[0] == 'd') && (CmdRx[1] == 'x') && (CmdRx[2] == 'u')){
             //CmdRx ="";
             Comando  = 5;
            } else if ((CmdRx[0] == 's') && (CmdRx[1] == 'x') && (CmdRx[2] == 'd')){
             //CmdRx ="";
             Comando  = 8;
            } else if ((CmdRx[0] == 's') && (CmdRx[1] == 'x') && (CmdRx[2] == 'u')){
             Comando  = 7;
            } else if ((CmdRx[0] == 'l') && (CmdRx[1] == 'e') && (CmdRx[2] == 'f')){
             //CmdRx ="";
             Comando  = 3;
            } else if ((CmdRx[0] == 'r') && (CmdRx[1] == 'i') && (CmdRx[2] == 'g')){
             //CmdRx ="";
             Comando  = 4;
            } else if ((CmdRx[0] == 's') && (CmdRx[1] == 't') && (CmdRx[2] == 'o')){
             //CmdRx ="";
             Comando  = 0;
            } else if ((CmdRx[0] == 'm') && (CmdRx[1] == 'c') && (CmdRx[2] == 'o')){
              //Comando  = 100;
              if (modo_motori  == MODO_CONTINUO)
                modo_motori  = MODO_IMPULSO;
              else
                modo_motori  = MODO_CONTINUO;

            } else if ((CmdRx[0] == 's') && (CmdRx[1] == 'c') && (CmdRx[2] == 'o')){
              //Comando  = 101;
              if (modo_servi  == MODO_COLPO)
                modo_servi  = MODO_IMPULSO;
              else
                modo_servi  = MODO_COLPO;
              
            } else 
              Comando = 250;
            
              
           #if EN_CONSOLE 
            Serial.write("Sito Inviato");
           #endif
				  
         stampaSito(client);
   #endif
					break;
				}
				if (c == '\n')
				{
					// you're starting a new line
					// inizio di una nuova linea
					currentLineIsBlank = true;
				}
				else if (c != '\r')
				{
					// you've gotten a character on the current line
					// sono stati ricevuti dei caratteri nella linea corrente
					currentLineIsBlank = false;
				}
			}
		}
		// give the web browser time to receive the data
		// lascia tempo al browser per ricevere i dati
		delay(1);

		// close the connection:
		// chiudi la connessione
		client.stop();
		//Serial.println("client disonnected");
		//Serial << F("Free RAM: ") << FreeRam() << "\n";

    delay(10);  // prova !!! matteo
 #if MY_WEB_APP
    /// framebot 
    ExecCommand(Comando);
 #endif

	}
	
}


/* Cerca la stringa:
 *   GET /btn=
 * 
 *  ed estrae il comando passato fino al carattere '?'
 *  Es.:
 *  GET /btn=stop?
 */
void EstraiGET(char ParCar)
{
  switch (MST_IdxExtractCMD){
    case 0:                     // cerca primo car: "b"
     if (ParCar == 'G')
        MST_IdxExtractCMD++;
     else
       MST_IdxExtractCMD = 0;
    break;
    case 1:                     // cerca primo car: "b"
     if (ParCar == 'E')
        MST_IdxExtractCMD++;
     else
       MST_IdxExtractCMD = 0;
    break;
    case 2:                     // cerca primo car: "b"
     if (ParCar == 'T')
        MST_IdxExtractCMD++;
     else
       MST_IdxExtractCMD = 0;
    break;
    case 3:                     // cerca primo car: "b"
     if (ParCar == ' ')
        MST_IdxExtractCMD++;
     else
       MST_IdxExtractCMD = 0;
    break;
    case 4:                     // cerca primo car: "b"
     if (ParCar == '/')
        MST_IdxExtractCMD++;
     else
       MST_IdxExtractCMD = 0;
    break;
    case 5:                     // cerca primo car: "b"
     if (ParCar == 'b')
        MST_IdxExtractCMD++;
     else
       MST_IdxExtractCMD = 0;
    break;
    case 6:                     // cerca primo car: "t"
     if (ParCar == 't')
        MST_IdxExtractCMD++;
     else
       MST_IdxExtractCMD = 0;
    break;
    case 7:                     // cerca primo car: "n"
     if (ParCar == 'n')
        MST_IdxExtractCMD++;
     else
       MST_IdxExtractCMD = 0;
    break;
    case 8:                     // cerca primo car: "n"
     if (ParCar == '=')
        MST_IdxExtractCMD++;
     else
       MST_IdxExtractCMD = 0;
    break;
    case 9:                     // cerca primo car: "b"
     //if (ParCar == '=')
     //   break;
     if (ParCar != '?')
        CmdRx += ParCar;
     else
       MST_IdxExtractCMD = 99;
    break;
    case 99:
    break;
    default:
        MST_IdxExtractCMD = 0;
        CmdRx = "";
    break;
  }
}


#define ARG_PAR_UP       5
#define ARG_PAR_STOP     7
#define ARG_PAR_DOWN     9
#define ARG_PAR_LEFT    11
#define ARG_PAR_STOP1   13
#define ARG_PAR_RIGHT   15
#define ARG_PAR_LUP     17
#define ARG_PAR_RUP     19
#define ARG_PAR_LDOWN   21
#define ARG_PAR_RDOWN   23
#define ARG_PAR_M_CONT  25
#define ARG_PAR_S_CONT  27

void stampaForm(FishinoClient client,uint8_t cmd, uint8_t parArg){

 FLASH_STRING_ARRAY(site_array, 
    PSTR("<td><form action=\"btn="), 
    PSTR("\" method=\"GET\"><input style=\"font-size : 20px; width: 100px; height: 100px;\" type=\"submit\" value=\""), 
    PSTR("\"></form> </td>\n"),
    PSTR("\" method=\"GET\"><input style=\" background-color: red; font-size : 20px; width: 100px; height: 100px;\" type=\"submit\" value=\""),
    PSTR("\" method=\"GET\"><input style=\" background-color: green; font-size : 20px; width: 100px; height: 100px;\" type=\"submit\" value=\""),
    PSTR("UP"),    // primo parametro bottoni. Aggiornare l'indice delle define in caso cambi il numero di righe sopra
    PSTR("up"),
    PSTR("STOP"),
    PSTR("stop"),
    PSTR("DOWN"),
    PSTR("down"),
    PSTR("LEFT"),
    PSTR("left"),
    PSTR("STOP 1"),
    PSTR("stop1"),
    PSTR("RIGHT"),
    PSTR("right"),
    PSTR("L UP"),
    PSTR("sxu"),
    PSTR("R UP"),
    PSTR("dxu"),
    PSTR("L DOWN"),
    PSTR("sxd"),
    PSTR("R DOWN"),
    PSTR("dxd"),
    PSTR("M CONT"),
    PSTR("mcont"),
    PSTR("S CONT"),
    PSTR("scont"),
    );

  site_array[0].print(client);
	//client.println(cmd+1);
  site_array[cmd+1].print(client);
  if (parArg == 0)
	 site_array[1].print(client);
  else if (parArg == 1) 
   site_array[3].print(client);
  else
   site_array[4].print(client);
	//client.println(cmd);
  site_array[cmd].print(client);
  site_array[2].print(client);
}

void stampaSito(FishinoClient client)
{
 FLASH_STRING_ARRAY(site_array, 
    PSTR("<!doctype html>"), 
    PSTR("<html><head></head><body><table border=\"0\" style=\"width:60\%\"><tr><td></td>"), 
    //PSTR("<td></td><td></td></tr><tr><td></td><td></td><td></td><td></td>"), 
    PSTR("<td></td><td></td></tr><tr><td></td>"), 
    PSTR("<td></td><td></td>"),
    PSTR("<td></td></tr><tr><td></td>"),
    PSTR("<td></td><td></td><td></td>"),
    PSTR("</tr><tr><td></td><td></td>"),
    PSTR("<td></td><td></td><td></td>"),
    PSTR("</tr></table></body></html>")
    );
  
  site_array[0].print(client);
  site_array[1].print(client);
	stampaForm(client,ARG_PAR_UP,0);
	if ( modo_motori == MODO_CONTINUO)
	  stampaForm(client,ARG_PAR_M_CONT,1);
  else
    stampaForm(client,ARG_PAR_M_CONT,2);
  if ( modo_servi == MODO_COLPO)
    stampaForm(client,ARG_PAR_S_CONT,1);
  else
    stampaForm(client,ARG_PAR_S_CONT,2);
	site_array[2].print(client);
  stampaForm(client,ARG_PAR_STOP,0);
  site_array[3].print(client);
  stampaForm(client,ARG_PAR_LEFT,0);
  stampaForm(client,ARG_PAR_STOP1,0);
  stampaForm(client,ARG_PAR_RIGHT,0);
  site_array[4].print(client);
  stampaForm(client,ARG_PAR_DOWN,0);
  stampaForm(client,ARG_PAR_LUP,0);
  site_array[5].print(client);
  stampaForm(client,ARG_PAR_RUP,0);
  site_array[6].print(client);
  stampaForm(client,ARG_PAR_LDOWN,0);
  site_array[7].print(client);
  stampaForm(client,ARG_PAR_RDOWN,0);
  site_array[8].print(client);
}

#if MY_WEB_APP
void setupRobot() {

  pinMode(MOT1_EN1, OUTPUT);
  pinMode(MOT1_EN2, OUTPUT);

  digitalWrite(MOT1_EN1, LOW);
  digitalWrite(MOT1_EN2, LOW);

  pinMode(MOT2_EN1, OUTPUT);
  pinMode(MOT2_EN2, OUTPUT);


  digitalWrite(MOT2_EN1, LOW);
  digitalWrite(MOT2_EN2, LOW);


  servoDx.attach(SRV_DX);  // connette servo 
  delay(300);
  servoSx.attach(SRV_SX);  // connette servo 
  delay(300);

   posServoDx = POSITION_DEFAULT; 
   posServoSx = POSITION_DEFAULT;
   servoDx.write(posServoDx);
   servoSx.write(posServoSx);

  modo_motori = MODO_CONTINUO;
  modo_servi  = MODO_COLPO;
}

uint8_t StatusRobot;
void ExecCommand(uint8_t c) {

uint8_t ripeti;
#if EN_CONSOLE
//Serial.println("set movimento");
#endif
do{
  ripeti = 0;
  if ( c == 100){
/*
    if (modo_motori  == MODO_CONTINUO)
      modo_motori  = MODO_IMPULSO;
    else
      modo_motori  = MODO_CONTINUO;
  */  
  } else if (c == 101) {
/*
    if (modo_servi  == MODO_COLPO)
      modo_servi  = MODO_IMPULSO;
    else
      modo_servi  = MODO_COLPO;
   
  */  
  } else  if (c== 'a'){ // motore 1 avanti   ---> Dx indie
    digitalWrite(MOT1_EN2, LOW);
    digitalWrite(MOT1_EN1, HIGH);
    #if EN_CONSOLE
    //Serial.println("motore 1 avanti");
    #endif
  } else if (c == 's'){ // motore 1 stop
    digitalWrite(MOT1_EN1, HIGH);
    digitalWrite(MOT1_EN2, HIGH);
    #if EN_CONSOLE
    //Serial.println("motore 1 stop");
    #endif
  } else if (c == 'd'){ // motore 1 indietro
    digitalWrite(MOT1_EN2, HIGH);
    digitalWrite(MOT1_EN1, LOW);
    #if EN_CONSOLE
    //Serial.println("motore 1 indietro");
    #endif
  } else if (c == 'z'){ // motore 2 avanti --> Sx avsnti
    digitalWrite(MOT2_EN1, LOW);
    digitalWrite(MOT2_EN2, HIGH);
    #if EN_CONSOLE
    //Serial.println("motore 2 avanti");
    #endif
  } else if (c == 'x'){ // motore 2 stop
    digitalWrite(MOT2_EN1, HIGH);
    digitalWrite(MOT2_EN2, HIGH);
    #if EN_CONSOLE
    //Serial.println("motore 2 stop");
    #endif
  } else if (c == 'c'){ // motore 2 indietro
    digitalWrite(MOT2_EN1, HIGH);
    digitalWrite(MOT2_EN2, LOW);
    #if EN_CONSOLE
    //Serial.println("motore 2 indietro");
    #endif
  } else if (c == 8){
    if (modo_servi  == MODO_COLPO)
      servoDx.write(120);
    else {
      posServoDx += 5;
      if (posServoDx >= 180)
       posServoDx = 180;
       servoDx.write(posServoDx);
    }
      
    #if EN_CONSOLE    
    //Serial.println("servoDx 120");
    #endif
  } else if (c == 7){
    if (modo_servi  == MODO_COLPO)
      servoDx.write(60);
    else {
      posServoDx -= 5;
      if (posServoDx <= 60)
       posServoDx = 60;
       servoDx.write(posServoDx);
    }
      
    #if EN_CONSOLE
    //Serial.println("servoDx 60");
    #endif
  } else if (c == 6){
    if (modo_servi  == MODO_COLPO)
      servoSx.write(60);
    else {
      posServoSx -= 5;
      if (posServoSx <= 60)
       posServoSx = 60;
      servoSx.write(posServoSx);
    }
      
    #if EN_CONSOLE
    //Serial.println("servoSx 120");
    #endif
  } else if (c == 5){
    if (modo_servi  == MODO_COLPO)
      servoSx.write(120);
    else {
      posServoSx += 5;
      if (posServoSx >= 180)
       posServoSx = 180;
       servoSx.write(posServoSx);
    }
    
    #if EN_CONSOLE
    //Serial.println("servoSx 60");
    #endif
  } else if (c == 1){     // robot  avanti
    digitalWrite(MOT2_EN1, LOW);  // motore 2 avanti --> Sx avanti
    digitalWrite(MOT2_EN2, HIGH);
    digitalWrite(MOT1_EN2, LOW);  // motore 1 avanti   ---> Dx indietro
    digitalWrite(MOT1_EN1, HIGH);    
    StatusRobot = 1;
    if (modo_motori != MODO_CONTINUO){
      delay(350);
      ripeti = 1;
      StatusRobot = 0;
      c = 0;
    }

    #if EN_CONSOLE
    //Serial.println("robot avanti");
    #endif
  } else if (c == 0){       // robot  stop
    digitalWrite(MOT2_EN1, LOW);  // motore 2 avanti --> Sx stop
    digitalWrite(MOT2_EN2, LOW);
    digitalWrite(MOT1_EN2, LOW);  // motore 1 avanti   ---> Dx stop
    digitalWrite(MOT1_EN1, LOW);

  
    StatusRobot = 0;    
    #if EN_CONSOLE
    //Serial.println("robot stop");
    #endif
  } else if (c == 2){       // robot  indietro
    digitalWrite(MOT2_EN1, HIGH);  // motore 2 avanti --> Sx indietro
    digitalWrite(MOT2_EN2, LOW);
    digitalWrite(MOT1_EN2, HIGH);  // motore 1 avanti   ---> Dx indietro
    digitalWrite(MOT1_EN1, LOW);    
    StatusRobot = 2;
    if (modo_motori != MODO_CONTINUO){
      delay(350);
      ripeti = 1;
      StatusRobot = 0;
      c = 0;
    }

    #if EN_CONSOLE
    //Serial.println("robot indietro");
    #endif
  } else if (c == 3){       // robot  sinistra
    digitalWrite(MOT2_EN1, HIGH);  // motore 2 avanti --> Sx indietro
    digitalWrite(MOT2_EN2, LOW);
    digitalWrite(MOT1_EN2, LOW);  // motore 1 avanti   ---> Dx avanti
    digitalWrite(MOT1_EN1, HIGH);
    delay(350);
    ripeti = 1;
    if (StatusRobot == 0)
      c = 0;
    else if (StatusRobot == 1)
      c = 1;
    else if (StatusRobot == 2)
      c = 2;
    else
      ripeti = 0;
    #if EN_CONSOLE   
    //Serial.println("robot sinistra");
    #endif
  } else if (c == 4){       // robot  destra
    digitalWrite(MOT2_EN1, LOW);  // motore 2 avanti --> Sx avanti
    digitalWrite(MOT2_EN2, HIGH);
    digitalWrite(MOT1_EN2, HIGH);  // motore 1 avanti   ---> Dx indietro
    digitalWrite(MOT1_EN1, LOW);    
    delay(350);
    ripeti = 1;
    if (StatusRobot == 0)
      c = 0;
    else if (StatusRobot == 1)
      c = 1;
    else if (StatusRobot == 2)
      c = 2;
    else
      ripeti = 0;
    #if EN_CONSOLE
    //Serial.println("robot sinistra");
    #endif
  } 
 } while (ripeti == 1);
}
#endif
