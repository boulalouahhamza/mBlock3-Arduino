/*!
 *  @file mBlockDHT.cpp
 *
 *  تنفيذ مكتبة DHT11 / DHT22 المخصصة لإضافة mBlock 3.
 *
 *  ملاحظات:
 *   - DHT11 يحتاج ثانية واحدة على الأقل بين القراءات، و DHT22 ثانيتين.
 *     لذلك تُخزَّن آخر قراءة وتُستخدم لثانيتين قبل إعادة القراءة.
 *   - DHT11 الحديث يعطي جزءًا عشريًا في البايتين 1 و 3، وتكون صفرًا
 *     في النسخ القديمة، لذلك لا يؤثر ذلك على النسخ القديمة.
 */

#include "mBlockDHT.h"

mBlockDHT::mBlockDHT(uint8_t pin, uint8_t type) {
  _pin = pin;
  _type = type;
  for (uint8_t i = 0; i < 5; i++) {
    _data[i] = 0;
  }
  _lastRead = 0;
  _lastTemperature = -999.0;
  _lastHumidity = -999.0;
  _hasReading = false;
}

void mBlockDHT::begin() {
  pinMode(_pin, INPUT_PULLUP);
}

bool mBlockDHT::_readSensor() {
  // تصفير البيانات
  _data[0] = _data[1] = _data[2] = _data[3] = _data[4] = 0;

  // إرسال إشارة البداية
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  if (_type == DHT_TYPE_22) {
    delay(1);        // DHT22: على الأقل 1ms
  } else {
    delay(20);       // DHT11: على الأقل 18ms
  }

  pinMode(_pin, INPUT_PULLUP);
  delayMicroseconds(40);

  // انتظار استجابة المستشعر: 80us منخفض ثم 80us مرتفع
  uint32_t timeout = 0;
  while (digitalRead(_pin) == HIGH) {
    if (++timeout > 20000) return false;
  }
  timeout = 0;
  while (digitalRead(_pin) == LOW) {
    if (++timeout > 20000) return false;
  }
  timeout = 0;
  while (digitalRead(_pin) == HIGH) {
    if (++timeout > 20000) return false;
  }

  // قراءة 40 بت
  for (int i = 0; i < 40; i++) {
    // انتظار نهاية النبضة المنخفضة (50us)
    timeout = 0;
    while (digitalRead(_pin) == LOW) {
      if (++timeout > 20000) return false;
    }

    // قياس طول النبضة المرتفعة
    uint32_t start = micros();
    timeout = 0;
    while (digitalRead(_pin) == HIGH) {
      if (++timeout > 20000) return false;
    }
    uint32_t duration = micros() - start;

    _data[i / 8] <<= 1;
    if (duration > 40) {
      _data[i / 8] |= 1;  // نبضة طويلة (~70us) = 1
    }
  }

  // التحقق من checksum
  return _data[4] == ((_data[0] + _data[1] + _data[2] + _data[3]) & 0xFF);
}

void mBlockDHT::_decode() {
  if (_type == DHT_TYPE_11) {
    // DHT11: رطوبة صحيحة + عشرية، حرارة صحيحة + عشرية
    _lastHumidity = (float)_data[0] + (float)_data[1] * 0.1;
    _lastTemperature = (float)_data[2] + (float)_data[3] * 0.1;
  } else {
    // DHT22: 16 بت لكل قيمة، والحرارة بإشارة
    _lastHumidity = (float)(((uint16_t)_data[0] << 8) | _data[1]) * 0.1;
    uint16_t rawTemperature = ((uint16_t)(_data[2] & 0x7F) << 8) | _data[3];
    _lastTemperature = (float)rawTemperature * 0.1;
    if (_data[2] & 0x80) {
      _lastTemperature = -_lastTemperature;
    }
  }
}

bool mBlockDHT::_update() {
  uint32_t now = millis();

  // استخدم القراءة المخزنة إذا كانت حديثة (أقل من ثانيتين).
  if (_hasReading && (uint32_t)(now - _lastRead) < 2000UL) {
    return true;
  }

  if (_readSensor()) {
    _decode();
    _lastRead = now;
    _hasReading = true;
    return true;
  }

  // فشلت القراءة: استخدم آخر قيمة صحيحة لمدة 30 ثانية كحد أقصى.
  if (_hasReading && (uint32_t)(now - _lastRead) < 30000UL) {
    return true;
  }

  return false;
}

bool mBlockDHT::readRaw() {
  if (!_readSensor()) {
    return false;
  }
  _decode();
  _lastRead = millis();
  _hasReading = true;
  return true;
}

float mBlockDHT::readTemperature() {
  if (!_update()) return -999.0;
  return _lastTemperature;
}

float mBlockDHT::readHumidity() {
  if (!_update()) return -999.0;
  return _lastHumidity;
}
