#define Interruptfrequency   200  //Interrupt frequenz (=Abtastfrequenz)
#define BufferSize 7500

// Globale flag Variablen für bufferFiltered definieren
bool flag1 = false; // first part of buffer raw
bool flag2 = false; // second part of buffer raw
bool flag3 = false; // first part of buffer for display
bool flag4 = false; // first part of buffer for display
bool BufferRawComplete = false;
bool BufferFilteredComplete = false;
bool writtenBufferRawAndFiltered = false;
uint16_t bufferRawAndFiltered[BufferSize*2];
uint16_t bufferFilteredForDisplay[BufferSize];
int16_t bufferIndex = 0; //for both buffers

 // TEST SIGNAL 
uint16_t triangule_array[7500]; //for the test Signal
int trianguleArrayValue = 0; // for the test signal

void fill_triangule_array() { //test signal
  for (int i = 0; i<7500; i++) {
    triangule_array[i] = trianguleArrayValue;
    trianguleArrayValue = trianguleArrayValue + 4;
    if (trianguleArrayValue >= 1024) { //30 peaks in 30 secons
      trianguleArrayValue=0;    
      }
    }
}


// global variables for the digital filter
double b0 = 1;
double b1 = -0.618082789968684487291739060310646891594;
double b2 = 1;
double a0 = 1;
double a1 = -0.610411726197078174926957672141725197434;
double a2 = 0.975177876180648883774892965448088943958;
double Mem0[1] = {0};
double Mem1[1] = {0};
double Mem2[1] = {0};
uint16_t data_RAW;
double data_flt;
double Filter_For_Display(int k, double x);

// digital filter
double Filter_For_Display(int k, double x) {
  double y;
  Mem0[k] = x - a1 * Mem1[k] - a2 * Mem2[k];
  y = b0 * Mem0[k] + b1 * Mem1[k] + b2 * Mem2[k];
  Mem2[k] = Mem1[k];
  Mem1[k] = Mem0[k];
  return y;
}

void IRAM_ATTR Einlesen() // each 4ms its executed
{


//--------------------------- ECG SignaL
  //data_RAW = triangule_array[bufferIndex]; // config for the test signal
  //Serial.printf("data_RAW: %d Triangule array[i]: %d \n", data_RAW, triangule_array[bufferIndex]);
  data_RAW = analogRead(34); //data from ADC signal (ecg electrodes)

  if (writtenBufferRawAndFiltered = false) { // written just once
    bufferRawAndFiltered[bufferIndex] = data_RAW; // buffer with data raw
    bufferRawAndFiltered[bufferIndex+7500] = Filter_For_Display(0, data_RAW); // buffer with the data filtered
     if(flag2 || flag4) {
       writtenBufferRawAndFiltered = true;
     }
  }
  //Serial.printf(" bufferRawAndFiltered[bufferIndex]: %d \n",  bufferRawAndFiltered[bufferIndex]);
  bufferFilteredForDisplay[bufferIndex] = Filter_For_Display(0, data_RAW); // buffer with data with filter
  bufferIndex++; // inc counter buffer
  bufferIndex = bufferIndex%BufferSize; //limit in the index of the bufffer, so it does not grow ad infinitum
//------------------------- end ECG Signal 

  if(bufferIndex == BufferSize/2) {
    flag1 = true; // first part of buffer raw is ready
    flag3 = true; // first part of buffer filtered is ready
    Serial.println("flag 1 Buffer Raw and filtered = true");
    Serial.println("flag 3 Buffer Raw and filtered = true");
  }

  if(bufferIndex == BufferSize-1) {
    flag2 = true; // second part of buffer raw is ready
    flag4 = true; // second part of buffer filtered is ready
    Serial.println("flag 2 Buffer raw and filtered = true");
    Serial.println("flag 4 Buffer raw and filtered = true");

  }
}

