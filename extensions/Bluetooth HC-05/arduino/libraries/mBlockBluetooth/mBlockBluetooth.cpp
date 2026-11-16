/*!
 *  @file mBlockBluetooth.cpp
 *
 *  تنفيذ مكتبة البلوتوث (HC-05 / HC-06) لإضافة mBlock 3.
 *
 *  كل الحروف القادمة من الهاتف تُقرأ عند استدعاء أي كتلة قراءة،
 *  لذلك لا تحتاج إلى حلقة قراءة منفصلة.
 */

#include "mBlockBluetooth.h"

// مقارنة نصين مع تجاهل المسافات الطرفية وحالة الأحرف.
static bool textEqualsIgnoreCase(const char *actual, const String &expected) {
  int start = 0;
  int end = (int)expected.length() - 1;

  while (start <= end &&
         (expected.charAt(start) == ' ' || expected.charAt(start) == '\t')) {
    start++;
  }
  while (end >= start &&
         (expected.charAt(end) == ' ' || expected.charAt(end) == '\t')) {
    end--;
  }

  int i = 0;
  for (int k = start; k <= end; k++) {
    char a = actual[i++];
    if (a == '\0') {
      return false;
    }

    char b = expected.charAt(k);
    if (a >= 'A' && a <= 'Z') {
      a = (char)(a - 'A' + 'a');
    }
    if (b >= 'A' && b <= 'Z') {
      b = (char)(b - 'A' + 'a');
    }

    if (a != b) {
      return false;
    }
  }

  return actual[i] == '\0';
}

mBlockBluetooth::mBlockBluetooth(uint8_t rxPin, uint8_t txPin)
    : _serial(rxPin, txPin) {
  _rxPin = rxPin;
  _txPin = txPin;

  _buffer[0] = '\0';
  _length = 0;

  _message[0] = '\0';
  _hasMessage = false;
  _newMessage = false;
  _delivered = false;
  _lastByteMs = 0;
}

void mBlockBluetooth::begin(long baud) {
  _serial.begin(baud);
  _serial.listen();

  _buffer[0] = '\0';
  _length = 0;
  _message[0] = '\0';
  _hasMessage = false;
  _newMessage = false;
  _delivered = false;
  _lastByteMs = millis();
}

void mBlockBluetooth::sendText(const String &text) {
  _serial.print(text);
}

void mBlockBluetooth::sendLine(const String &text) {
  _serial.println(text);
}

void mBlockBluetooth::poll() {
  // بعد تسليم رسالة، نتوقف عن القراءة حتى يطلب البرنامج الرسالة التالية.
  // بهذا لا تمسح رسالة سريعة رسالةً لم تُقرأ بعد.
  if (_delivered) {
    return;
  }

  while (_serial.available() > 0) {
    char c = (char)_serial.read();
    _lastByteMs = millis();

    // نهاية السطر: الرسالة اكتملت.
    if (c == '\n' || c == '\r') {
      if (_length > 0) {
        finishMessage();
      }
      continue;
    }

    // المخزن ممتلئ: نسلّم ما لدينا ثم نبدأ من جديد.
    if (_length >= MBLOCK_BT_BUFFER - 1) {
      finishMessage();
    }

    _buffer[_length++] = c;
  }

  // بعض التطبيقات ترسل بدون سطر جديد، لذلك نتوقف عند سكوت قصير.
  if (_length > 0 && (millis() - _lastByteMs) > MBLOCK_BT_TIMEOUT_MS) {
    finishMessage();
  }
}

void mBlockBluetooth::finishMessage() {
  // تجاهل المسافات في البداية والنهاية.
  while (_length > 0 &&
         (_buffer[_length - 1] == ' ' || _buffer[_length - 1] == '\t')) {
    _length--;
  }

  uint8_t start = 0;
  while (start < _length &&
         (_buffer[start] == ' ' || _buffer[start] == '\t')) {
    start++;
  }

  uint8_t count = (uint8_t)(_length - start);
  if (count >= MBLOCK_BT_BUFFER) {
    count = MBLOCK_BT_BUFFER - 1;
  }

  for (uint8_t i = 0; i < count; i++) {
    _message[i] = _buffer[start + i];
  }
  _message[count] = '\0';

  _length = 0;
  _buffer[0] = '\0';

  if (count > 0) {
    _hasMessage = true;
    _newMessage = true;
  }
}

bool mBlockBluetooth::messageReady() {
  _delivered = false;
  poll();

  if (_newMessage) {
    _newMessage = false;
    _delivered = true;
    return true;
  }

  return false;
}

String mBlockBluetooth::readMessage() {
  poll();

  if (!_hasMessage) {
    return String("");
  }

  return String(_message);
}

bool mBlockBluetooth::messageMatches(const String &expected) {
  poll();

  if (!_hasMessage) {
    return false;
  }

  return textEqualsIgnoreCase(_message, expected);
}

long mBlockBluetooth::readNumber() {
  poll();

  if (!_hasMessage) {
    return 0;
  }

  const char *p = _message;
  while (*p == ' ' || *p == '\t') {
    p++;
  }

  bool negative = false;
  if (*p == '-') {
    negative = true;
    p++;
  } else if (*p == '+') {
    p++;
  }

  long value = 0;
  bool found = false;
  while (*p >= '0' && *p <= '9') {
    value = value * 10 + (*p - '0');
    found = true;
    p++;
  }

  if (!found) {
    return 0;
  }

  return negative ? -value : value;
}

void mBlockBluetooth::clearMessage() {
  _buffer[0] = '\0';
  _length = 0;
  _message[0] = '\0';
  _hasMessage = false;
  _newMessage = false;
  _delivered = false;
}
