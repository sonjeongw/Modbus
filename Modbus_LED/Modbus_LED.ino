#include <SimpleModbusSlave.h>

const int ledPin = 2;      // 첫 번째 LED가 연결된 디지털 핀 blue
const int ledPin2 = 5;     // 두 번째 LED가 연결된 디지털 핀 red
const int switchPin = 0;   // 스위치가 연결된 디지털 핀

// 모드버스 레지스터 정의
enum {
  SWITCH_STATUS = 0,        // 스위치 상태를 저장하는 레지스터
  LED_STATUS = 1,           // 첫 번째 LED 상태를 저장하는 레지스터
  LED_STATUS_2 = 2,         // 두 번째 LED 상태를 저장하는 레지스터
  TOTAL_REGS_SIZE = 3       // 전체 레지스터의 개수
};

unsigned int holdingRegs[TOTAL_REGS_SIZE];  // 모드버스 레지스터 배열

// LED 및 스위치 핀을 설정하고, 모드버스 통신을 초기화
void setup() {
  pinMode(ledPin, OUTPUT);    // blue LED 핀을 출력으로 설정
  pinMode(ledPin2, OUTPUT);   // red LED 핀을 출력으로 설정
  pinMode(switchPin, INPUT_PULLUP);  // 스위치 핀을 풀업 입력으로 설정

  // 모드버스 설정
  // - &Serial: 사용할 시리얼 통신 포트
  // - 9600: 통신 속도
  // - SERIAL_8N2: 8비트 데이터, No parity, 2 stop bits
  // - 1: 슬레이브의 모드버스 주소
  // - 0: 디바이스 ID (마스터 또는 슬레이브를 구분하기 위한 ID, 여기서는 사용되지 않음)
  // - TOTAL_REGS_SIZE: 레지스터 배열 크기
  // - holdingRegs: 레지스터 배열
   modbus_configure(&Serial, 9600, SERIAL_8N2, 1, 0, TOTAL_REGS_SIZE, holdingRegs);
}

// 주기적으로 모드버스를 업데이트하고 스위치의 상태를 읽어와 LED를 제어
void loop() {
  // 모드버스 업데이트
  modbus_update();

  // 스위치 상태를 모드버스 레지스터에 저장
  holdingRegs[SWITCH_STATUS] = digitalRead(switchPin);

  // 모드버스 레지스터 값에 따라 LED 제어
  if (holdingRegs[SWITCH_STATUS] == HIGH) {
    holdingRegs[LED_STATUS] = LOW;     // 스위치가 HIGH(눌렸을 때)이면 첫 번째 LED를 끔
    holdingRegs[LED_STATUS_2] = HIGH;  // 스위치가 HIGH(눌렸을 때)이면 두 번째 LED를 켬
  } else {
    holdingRegs[LED_STATUS] = HIGH;    // 스위치가 LOW(안 눌렸을 때)이면 첫 번째 LED를 켬
    holdingRegs[LED_STATUS_2] = LOW;   // 스위치가 LOW(안 눌렸을 때)이면 두 번째 LED를 끔
  }

  // 모드버스 레지스터에 따라 LED 제어
  digitalWrite(ledPin, holdingRegs[LED_STATUS]);
  digitalWrite(ledPin2, holdingRegs[LED_STATUS_2]);
}