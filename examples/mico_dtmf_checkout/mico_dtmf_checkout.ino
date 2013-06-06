/*
Test receiving DTMF tones using interrupts
 */

char data_ready_status=0, DTMF_RX_BUFFER=0;
char dr_serviced=0;

int dr = 2; // dig 2
int d0 = A5; // analog 5
int d1 = A4; // analog 4
int d2 = 5; // dig 5
int d3 = 7; // dig 7

void setup()
{
 
 // initialize the digital pin as an output.
    pinMode(d0, INPUT);    
    pinMode(d1, INPUT); 
    pinMode(d2, INPUT); 
    pinMode(d3, INPUT); 
    pinMode(dr, INPUT);    
    
 // Open serial communications and wait for port to open:
  Serial.begin(9600);

  Serial.print("Waiting for DTMF...");

  attachInterrupt(0, data_ready_int , RISING);
}


void loop(void) {   
char tmp; //grab data from buffer

  if (data_ready_status) 
  {  
      data_ready_status =0;
             
      tmp = dtmf_ascii(DTMF_RX_BUFFER);  //convert raw instream into ascii
      Serial.print("DTMF digit received: ");
      Serial.println(tmp); 
  }
}

// ----DTMF receive functions ----
void data_ready_int()  //interrupt service routine
{
      data_ready_status = 1; 
      DTMF_RX_BUFFER = dtmf_get_data(); //read 4 dig ins and put into byte. Need to do now before it vanishes
}





 
 
 //--- the stuff below can go to dtmf_lib

  
  // ----DTMF receive functions ----
  char dtmf_get_data ( void  )  //read data port and assemble the bits into a digit
  {
  // dtmf digit is decoded per Page 5 Table 1 of chip datasheet
  // http://www.zarlink.com/zarlink/mt8870d-datasheet-oct2006.pdf
    char data[4] = {0,0,0,0};
          
    data[0] = digitalRead(d0);
    data[1] = digitalRead(d1);
    data[2] = digitalRead(d2);
    data[3] = digitalRead(d3);
          
    return  ( 8* data[3] +  4* data[2] + 2* data[1] + data[0] ) ;
    
  }

char dtmf_ascii (char dtmf_raw)
{

    switch (dtmf_raw)
    {
    case 10:
      return '0';
    case 11:
        return '*';
    case 12:
        return '#';
    case 13:
        return 'A';
    case 14:
        return 'B';
    case 15:
        return 'C';
    case 0:
        return 'D';
    default:
        return (dtmf_raw+48);
    }
}

char dtmf_ascii2dec (char dtmf_raw)
{
  if ((dtmf_raw >= '0') && (dtmf_raw <= '9'))
    return (dtmf_raw-48);
  else
    return 0xff;
}  

 





