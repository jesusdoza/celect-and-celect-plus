

//GLOBAL VAR

int crankPin=3;// uses cam shaft for timing on nano we are using 3
int startPin=12;



int crankRatio=23;  //how many regular pulses to one sync pulse 0 counts so using -1 to set up correctly 23 works messing with 28
int syncPulseRepeats=3;// how many sync mini pulses occur 3 works
int syncDivider=2;// 3 works

int idleSpacing=6240; //800 rpm
int midleRangeSpacing=3500;//3500

//long pulseSpacing=6240;// 6240 800 rpm 3500 working spacing between highs of each pulse //in micros 2500 micros=1978 RPM, 10,000 micros = 500 RPM  somewhat formula (100,000/(wanted rpm/50)) = wanted micros for RPM
//long pulseSpacing=midleRangeSpacing;
long pulseSpacing=idleSpacing;


//long highTriggerLength=pulseSpacing*.5; //how long high state lasts pulsespacing * .5 works
long lowTriggerLength=pulseSpacing*.5; 


 
bool triggerOccured=false;
bool sync=false;// sync event needs to happen
bool lowActive=false;

int crankCounter=0;// keep track of pulses will reset after sync event

long long triggerTime=0; //when should event happen in micros






void setup() {
  // put your setup code here, to run once:
pinMode(crankPin,OUTPUT);
pinMode(startPin,INPUT_PULLUP);
}

void loop() {
// certain amount of pulses after crank ratio is met
//regular pulse event
if(crankCounter<crankRatio){
  sync=false;
}
//sync event has ended
else if(crankCounter>=(crankRatio+syncPulseRepeats)){
  sync=false;
  crankCounter=0;
}
//sync event 
else if(crankCounter>=(crankRatio)){
  sync=true;
}
else{
  //do nothing catch 
}



//****************************************TIMING SET UP*****************************************
//sets up timing once a trigger occurs and only happen once per trigger window
if(triggerOccured==true){  

 //normal trigger event timing
 if(sync==false){
   triggerTime=(micros()+pulseSpacing);
   triggerOccured=false;// set flag to false to allow trigger to happen this loop only
 }

 //sync event is needed and timing must change to 3rd
 else if(sync==true){
  // triggerTime=(micros()+(pulseSpacing/syncDivider));//orig
  triggerTime=(micros()+(pulseSpacing*syncDivider));
  triggerOccured=false;// set flag to false to allow trigger to happen this loop only
 }

}

//*****************************************TIMING SET UP ABOVE*****************************************





// trigger time and also trigger has not occured this iteration only happens once per time window
//********************************************************************
//if else if block for either high or low
if((micros()>=triggerTime)&& (triggerOccured==false)){
 
  triggerPulse(crankPin);
  triggerOccured=true;
  lowActive=true;
  //crankCounter++;// increment counter since a pulse happened 
  

}
//else check trigger time for it to go low 
//low will always happen before trigger time 
else if (micros()>=(triggerTime-lowTriggerLength)&&(lowActive)){
  
    endPulse(crankPin);
    crankCounter++;
    lowActive=false;

}
//*************************************************************************



}


//FUNCTIONS BELOW**************************************************************

void triggerPulse(int pin_){
  digitalWrite(pin_,HIGH);
  return;
}
void endPulse(int pin_){
  digitalWrite(pin_,LOW);
  return;
}
