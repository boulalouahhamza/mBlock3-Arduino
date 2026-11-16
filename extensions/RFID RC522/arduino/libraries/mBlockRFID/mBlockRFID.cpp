/*!
 *  @file mBlockRFID.cpp
 *
 *  تنفيذ طبقة التبسيط فوق مكتبة MFRC522 لإضافة mBlock 3.
 */

#include "mBlockRFID.h"

// يحوّل قيمة 0..15 إلى حرف سداسي عشري كبير.
static char rfidHexDigit(uint8_t value) {
  return (value < 10) ? (char)('0' + value) : (char)('A' + value - 10);
}

mBlockRFID::mBlockRFID(uint8_t ssPin, uint8_t rstPin) : _rfid(ssPin, rstPin) {
  _cardReady = false;
  _hasUID = false;
  _uidLen = 0;
}

void mBlockRFID::begin() {
  // مهم: تهيئة ناقل SPI (اتجاهات أطراف SCK/MOSI وتمكينه) قبل أي اتصال بالشريحة.
  // بدون هذا السطر لا تصل أي إشارة إلى RC522 ولا تُقرأ البطاقة.
  SPI.begin();
  _rfid.PCD_Init();
  delay(5);
  _cardReady = false;
  _clearUID();
}

void mBlockRFID::_clearUID() {
  _hasUID = false;
  _uidLen = 0;
}

bool mBlockRFID::cardPresent() {
  _cardReady = _rfid.PICC_IsNewCardPresent();
  if (_cardReady) {
    // بطاقة جديدة: UID القديم لم يعد صالحًا.
    _clearUID();
  }
  return _cardReady;
}

bool mBlockRFID::_readUIDBytes() {
  // إذا نُفِّذت كتلة "card present" قبل القراءة نستخدم نتيجتها مباشرة،
  // وإلا نفحص وجود بطاقة الآن.
  if (!_cardReady && !_rfid.PICC_IsNewCardPresent()) {
    return false;
  }
  _cardReady = false;
  if (!_rfid.PICC_ReadCardSerial()) {
    return false;
  }

  _uidLen = _rfid.uid.size;
  if (_uidLen > sizeof(_uidBuf)) {
    _uidLen = sizeof(_uidBuf);
  }
  for (uint8_t i = 0; i < _uidLen; i++) {
    _uidBuf[i] = _rfid.uid.uidByte[i];
  }

  // إيقاف البطاقة الآن، لأن القراءة التالية تحتاج بطاقة جديدة.
  _rfid.PICC_HaltA();
  return true;
}

bool mBlockRFID::_ensureUID() {
  if (_hasUID) {
    return true;
  }
  if (!_readUIDBytes()) {
    return false;
  }
  _hasUID = true;
  return true;
}

String mBlockRFID::readUID() {
  if (!_ensureUID()) {
    return String("");
  }

  String result = "";
  for (uint8_t i = 0; i < _uidLen; i++) {
    result += rfidHexDigit((uint8_t)(_uidBuf[i] >> 4));
    result += rfidHexDigit((uint8_t)(_uidBuf[i] & 0x0F));
    if (i + 1 < _uidLen) {
      result += " ";
    }
  }
  return result;
}

unsigned long mBlockRFID::readUIDNumber() {
  if (!_ensureUID()) {
    return 0;
  }

  unsigned long number = 0;
  for (uint8_t i = 0; i < _uidLen; i++) {
    number = (number << 8) | _uidBuf[i];
  }
  return number;
}

bool mBlockRFID::uidMatches(const String &expected) {
  if (!_ensureUID()) {
    return false;
  }

  // نحوّل النص المطلوب إلى صيغة موحّدة: بلا مسافات ولا فواصل وبأحرف كبيرة.
  String wanted = "";
  for (unsigned int i = 0; i < expected.length(); i++) {
    char c = expected.charAt(i);
    if (c == ' ' || c == ':' || c == '-' || c == '_' || c == '.') {
      continue;
    }
    if (c >= 'a' && c <= 'f') {
      c = (char)(c - 'a' + 'A');
    }
    if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F')) {
      wanted += c;
    }
  }

  if (wanted.length() == 0) {
    return false;
  }

  String actual = "";
  for (uint8_t i = 0; i < _uidLen; i++) {
    actual += rfidHexDigit((uint8_t)(_uidBuf[i] >> 4));
    actual += rfidHexDigit((uint8_t)(_uidBuf[i] & 0x0F));
  }

  return wanted == actual;
}

void mBlockRFID::halt() {
  _rfid.PICC_HaltA();
  _cardReady = false;
  _clearUID();
}
