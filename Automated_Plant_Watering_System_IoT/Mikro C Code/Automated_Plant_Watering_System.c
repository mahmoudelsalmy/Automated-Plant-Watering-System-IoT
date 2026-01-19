// =========================
// LCD Pins
// =========================
sbit LCD_RS at RD2_bit;
sbit LCD_EN at RD3_bit;
sbit LCD_D4 at RD4_bit;
sbit LCD_D5 at RD5_bit;
sbit LCD_D6 at RD6_bit;
sbit LCD_D7 at RD7_bit;

sbit LCD_RS_Direction at TRISD2_bit;
sbit LCD_EN_Direction at TRISD3_bit;
sbit LCD_D4_Direction at TRISD4_bit;
sbit LCD_D5_Direction at TRISD5_bit;
sbit LCD_D6_Direction at TRISD6_bit;
sbit LCD_D7_Direction at TRISD7_bit;

// =========================
// Vars
// =========================
unsigned long soil_raw, tank_raw;
unsigned long soil_percent, tank_percent;
char pump_status = 0;

char command;




// =========================
// Init ADC
// =========================
void ADC_Init_Custom() {
    ADCON1 = 0x80;
    TRISA = 0xFF;
}

// =========================
// ADC Read
// =========================
unsigned int ADC_Read_Custom(char channel) {
    ADCON0 = (channel << 3) | 0x81;
    ADCON1 |= (1<<6);
    delay_ms(5);
    GO_DONE_bit = 1;
    while(GO_DONE_bit);
    return ((ADRESH << 8) + ADRESL);
}

// =========================
// Pump Control
// =========================
void Pump_On() { RC0_bit = 1; pump_status = 1; }
void Pump_Off() { RC0_bit = 0; pump_status = 0; }

// =========================
// UART Send Format:
// <21,53,1>
// =========================
char needToSend = 0;


void willSend(){
  PORTB.F6 = 1; //pic want to send
  needToSend = 1;
}
  void removeSpaces(char *str) {
    int i = 0, j = 0;
    while (str[i] != '\0') {
        if (str[i] != ' ') {
            str[j++] = str[i];
        }
        i++;
    }
    str[j] = '\0'; // Null-terminate the modified string
}


int soilThreshold = 30;
     //format = #tank,soil,thres!
char sendDt[15] ;
char tanks[4];
char soils[4];
char thress[4];
void sendTx(unsigned long tank, unsigned long soil){

     if(needToSend && PORTB.F5/*esp ready*/){
          ByteToStr((char)tank, tanks);
          ByteToStr((char)soil, soils);
          ByteToStr((char)soilThreshold, thress);
          
          memset(sendDt, '\0', 15);
          strcpy(sendDt, "#");
          strcat( sendDt, tanks);
          strcat( sendDt, ",");
          strcat( sendDt, soils);
          strcat( sendDt, ",");
          strcat( sendDt, thress);
          strcat( sendDt, "!");
          
          removeSpaces(sendDt);
          UART1_Write_Text(sendDt);

          needToSend = 0;
          PORTB.F6 = 0; //pic not want to send

     }

}
char receivedDt[25];
int pmode = 2;
char * ptr = 0;
unsigned int t = 0;
char temp[5];
void receiveAndParse() {
    if(PORTB.F4 == 1) { // ESP wants to send
        PORTB.F6 = 0;
        PORTB.F7 = 1;   // PIC ready
        t = 0;
        memset(receivedDt, '\0', 24);
        while(!UART1_Data_Ready() && t < 500) { // wait up to 500 ms
            delay_ms(1);
            t++;
        }

        if(UART1_Data_Ready()) {
            UART1_Read_Text(receivedDt, "!", 24);
            receivedDt[24] = '\0';
            ptr = strchr(receivedDt, '#');
            
            if(ptr != 0) {
                pmode = atoi(ptr + 1);

            }
            ptr++;
            ptr = strchr(ptr, ',');
            if(ptr != 0) {
                soilThreshold = atoi(ptr + 1);
            }
        }

        PORTB.F7 = 0;   // PIC done
    }
}





// =========================
// MAIN
// =========================


char datas[7];
void main() {

    TRISC = 0b11111110;
    RC0_bit = 0;
    
    TRISB.F5 = 1;    //esp ready to recieve
    TRISB.F4 = 1;    //esp want to send
    TRISB.F7 = 0;    //pic ready to rec
    TRISB.F6 = 0;    //pic want to send

    ADC_Init_Custom();
    UART1_Init(115200);
    Delay_ms(200);

    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);
    //UART1_Write_Text("helloWorld");
    while(1) {

        // Read Sensors
        soil_raw = ADC_Read_Custom(0);
        tank_raw = ADC_Read_Custom(1);

        soil_percent = ((soil_raw * 100) / 1023);
        tank_percent = ((tank_raw * 100) / 1023);
        willSend();
        sendTx(tank_percent, soil_percent);
        receiveAndParse();

        // AUTO mode
        if(soil_percent < soilThreshold && tank_percent > 15 && pmode == 2) Pump_On();
        else if(pmode == 1) Pump_On();     // FORCE ON
        else if(pmode == 0) Pump_Off();    // FORCE OFF
        else if(pmode == 2) Pump_Off();//auto off



        // LCD Output
        Lcd_Cmd(_LCD_CLEAR);

        Lcd_Out(1,1,"Soil:");
        ByteToStr((char)soil_percent, datas);
        Lcd_Out_Cp(datas);
        Lcd_Chr_CP('%');
        Lcd_Out_CP("  ");
        if( soil_percent< soilThreshold)
             Lcd_Out_CP("DRY");
        else
            lcd_Out_cp("WET");

        Lcd_Out(2,1,"Tank:");
        ByteToStr((char)tank_percent, datas);
        Lcd_Out_Cp(datas);
        Lcd_Chr_CP('%');

        if(pump_status)
            Lcd_Out(2,10,"PMP:ON");
        else
            Lcd_Out(2,10,"PMP:OFF");


        Delay_ms(500);
    }
}