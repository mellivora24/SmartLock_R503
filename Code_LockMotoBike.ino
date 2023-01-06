/***********************************
 Khai bao TV, dinh hinh giao tiep, khoi tao id
***********************************/
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

//Tao ket noi voi cong Serial
SoftwareSerial finger_Serial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&finger_Serial);  //Tao ket noi giua sensor va board

const int coi = 5;
const int relay = 4;

const int button_1 = 11; //Them van tay   moi
const int button_2 = 12; //Xoa toan bo van tay trong bo nho

uint8_t id; // chon kieu du lieu nay vi id (0 -> 127) trong khi uint8_t (0-225)
uint8_t dem = 0;

/************************************
      Nhap vao 1 ID
************************************/
uint8_t IDnumber(){
  uint8_t num = 0;
  uint8_t Template;

  Template = finger.getTemplateCount();
  num = Template++;
  return num;
}
/************************************
      Phat am thanh
************************************/
void beep(){
  digitalWrite(coi, 1);
  delay(200);
  digitalWrite(coi, 0);
}
void incorrect(){
  digitalWrite(coi, 1);
  delay(200);
  digitalWrite(coi, 0);
  delay(500);
  digitalWrite(coi, 1);
  delay(200);
  digitalWrite(coi, 0);
}
/************************************
      Them dau van tay moi
************************************/
bool addFinger(){
  Serial.println("Them van tay, dat van tay cu vao!");
  while(!findFinger()){}
  Serial.println("Mat khau dung!");
  Serial.println("Dat van tay moi vao!");
  beep();
  finger.LEDcontrol(FINGERPRINT_LED_BREATHING, 50, FINGERPRINT_LED_PURPLE);
  delay(5000);
  id = IDnumber();
  //Dinh dang van tay lan 1
  int p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    if (p == FINGERPRINT_OK){
      beep();
    }else{
      incorrect();
    }
  }

  //Trich xuat dac trung van tay lan 1
  p = finger.image2Tz(1);
  if (p == FINGERPRINT_OK){
    beep();
  }else{
    incorrect();
    return false;
  }

  //Dinh dang van tay lan 2
  p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    if (p == FINGERPRINT_OK){
      beep();
    }else{
      incorrect();
    }
  }

  //Trich xuat dac trung van tay lan 2
  p = finger.image2Tz(2);
  if (p == FINGERPRINT_OK){
    beep();
  }else{
    incorrect();
    return false;
  }
  //Luu dac trung vao bo nho
  p = finger.createModel();
  if (p == FINGERPRINT_OK){
    beep();
  }else{
    incorrect();
    return false;
  }

  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK){
    Serial.println("Da them van tay!");
    finger.LEDcontrol(FINGERPRINT_LED_BREATHING, 50, FINGERPRINT_LED_BLUE);
    beep();
    return true;
  }else{
    Serial.println("Khong the them!");
    finger.LEDcontrol(FINGERPRINT_LED_BREATHING, 50, FINGERPRINT_LED_RED);
    incorrect();
    return false;
  }
  return true;
}
/************************************
      Them van tay moi trong 
        TH chua dat pass
************************************/
bool Finger_DataBaseEmpty(){
  Serial.println("Chua co van tay trong danh sach, them moi!");
  beep();
  id = 1;
  //Dinh dang van tay lan 1
  int p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    if (p == FINGERPRINT_OK){
      beep();
    }
  }
  Serial.println("Tim thay van tay!");
  //Trich xuat dac trung van tay lan 1
  p = finger.image2Tz(1);
  if (p == FINGERPRINT_OK){
    beep();
  }else{
    incorrect();
    return false;
  }

  //Dinh dang van tay lan 2
  p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    if (p == FINGERPRINT_OK){
      beep();
    }else{
      incorrect();
    }
  }

  //Trich xuat dac trung van tay lan 2
  p = finger.image2Tz(2);
  if (p == FINGERPRINT_OK){
    beep();
  }else{
    incorrect();
    return false;
  }
  //Luu dac trung vao bo nho
  p = finger.createModel();
  if (p == FINGERPRINT_OK){
    beep();
  }else{
    incorrect();
    return false;
  }

  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK){
    beep();
  }else{
    incorrect();
    return false;
  }
  return true;
}
/************************************
      Kiem tra dau van tay
************************************/
bool findFinger(){
  int p = -1;
  if (dem >= 5){
    Serial.println("Qua 5 lan, thu lai sau 30s.");
    digitalWrite(coi, 1);
    finger.LEDcontrol(FINGERPRINT_LED_BREATHING, 50, FINGERPRINT_LED_RED);
    delay(30000);
    digitalWrite(coi, 0);
    dem = 0;
  }
  //Yeu cau dat ngon tay vao cam bien va chup hinh ngon tay
  beep();
  Serial.println("Dat ngon tay vao cam bien.");
  while (finger.getImage() != FINGERPRINT_OK){
    //Kiem tra hinh vua chup
    p = finger.getImage();
    if (p == FINGERPRINT_OK){
      break;
    }else{
      incorrect();
      return false;
    }
  }
  Serial.println("Tim thay van tay!");
  //Lay dac diem van tay
  p = finger.image2Tz();
  if (p == FINGERPRINT_OK){
    beep();
  }else{
    incorrect();
    return false;
  }
  // Tim kiem van tay trong danh sach da luu
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK){
    Serial.println("Ok!");
    beep();
    return true;
  }else{
    Serial.println("Sai!");
    dem++;
    incorrect();
    delay(2000);
    return false;
  }
  return true;
}
/************************************
    Kiem tra xem ban nhan nut nao 
************************************/
uint8_t button_Press(){
  uint8_t button = 0;
  if (digitalRead(button_1)==0){
    Serial.println("Ban da an nut 1!");
    button = 1;
    return button;
  }else{
    if (digitalRead(button_2)==0){
      Serial.println("Ban da an nut 2!");
      button = 2;
      return button;
    }else{
          button = 0;
          return button;
    }
  }
}
/************************************
            Khoi dong
************************************/
void setup()
{
  //Khoi dong relay o muc cao, khoa xe
  pinMode(relay, OUTPUT);
  digitalWrite(relay, 1);

  Serial.begin(9600);
  //Khoi tao giao tiep giua cam bien va board (Toc do: 57600)
  finger.begin(57600);
  //Nhap nhay den cam bien
  finger.LEDcontrol(FINGERPRINT_LED_BREATHING, 100, FINGERPRINT_LED_RED);

  pinMode(coi, OUTPUT);
  digitalWrite(coi, 0);

  pinMode(button_1, INPUT_PULLUP);
  pinMode(button_2, INPUT_PULLUP);
  
  beep();

  //Kiem tra cam bien
  while (!finger.verifyPassword()){}
  if (finger.verifyPassword()){
    Serial.println("Cam bien da hoat dong!");
    beep();
  }
  LAB: 

  finger.getTemplateCount();
  Serial.print("Hien tai co: "); Serial.print(finger.templateCount); Serial.println(" van tay.");
  //Kiem tra da co dau van tay nao chua, neu chua thi them moi
  if (finger.templateCount == 0){
    while (!Finger_DataBaseEmpty()){}
    goto LAB;
  }
  LAB_C:
  //Cho doi dat ngon tay vao cam bien hoac nhan 1 nut
  while ((finger.getImage() != FINGERPRINT_OK) && (button_Press() == 0)){}
  if (finger.getImage() == FINGERPRINT_OK){
    while (!findFinger()){goto LAB_C;}
  }else{
    switch(button_Press()){
      case 1:
        while (!addFinger()){} //Them 1 dau van tay moi
        break;
      case 2:
        finger.LEDcontrol(FINGERPRINT_LED_BREATHING, 50, FINGERPRINT_LED_RED);
        while (!findFinger()){} //Phai kiem tra dau van tay truoc khi xoa du lieu
        finger.emptyDatabase(); //Xoa toan bo
        goto LAB;
    }
  }
}
//Chuong trinh chinh --> mo khoa
void loop(){
  digitalWrite(relay, 0);
  finger.LEDcontrol(FINGERPRINT_LED_ON, 0, FINGERPRINT_LED_BLUE);
  Serial.println(finger.getTemplateCount());
}