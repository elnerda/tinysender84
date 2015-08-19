

float temp=22.5;
uint32_t pres=99051;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
//analogReference(INTERNAL);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0){
    char var=Serial.read();
     switch(var){
        case 't':
        Serial.println(temp);
        break;
        case 'p':
        Serial.println(pres);
        break;
      } 
  }
 
  delay(1000);
}
 
 
 

