/*!
 *  @file mBlockIR.cpp
 *
 *  تنفيذ مكتبة استقبال الأشعة تحت الحمراء (VS1838B) لإضافة mBlock 3.
 *
 *  كيف يعمل مستشعر VS1838B؟
 *   - خرجه يبقى HIGH في حالة السكون.
 *   - عند وصول نبضة تحت حمراء (38kHz) يصبح الخرج LOW.
 *   - لذلك "النبضة" (mark) = LOW، و"الفراغ" (space) = HIGH.
 *
 *  بروتوكول NEC:
 *   - بداية الإطار: 9000us LOW ثم 4500us HIGH.
 *   - كل بت: 560us LOW ثم فراغ:
 *       560us  = 0
 *       1690us = 1
 *   - 32 بت تُرسل LSB أولًا: عنوان + ~عنوان + أمر + ~أمر.
 *   - إطار التكرار: 9000us LOW ثم 2250us HIGH ثم 560us LOW.
 *
 *  القراءة تتم عبر مقاطعة خارجية (INT0 على الطرف 2، INT1 على الطرف 3)
 *  حتى لا تفوت الضغطة أثناء انشغال البرنامج بكتل أخرى.
 */

#include "mBlockIR.h"

// حدود التسامح بالميكروثانية (أسماء واضحة بدل الأرقام السحرية).
#define IR_LEADER_MARK_MIN   6500UL
#define IR_LEADER_MARK_MAX  10000UL
#define IR_LEADER_SPACE_MIN  3500UL
#define IR_LEADER_SPACE_MAX  5500UL
#define IR_REPEAT_SPACE_MIN  1800UL
#define IR_REPEAT_SPACE_MAX  3000UL
#define IR_BIT_MARK_MIN       250UL
#define IR_BIT_MARK_MAX       900UL
#define IR_BIT_ZERO_MAX       900UL
#define IR_BIT_ONE_MIN       1200UL
#define IR_BIT_ONE_MAX       2200UL

// حالات فك الترميز.
#define IR_STATE_IDLE        0
#define IR_STATE_AFTER_LEAD  1
#define IR_STATE_READY_BITS  2
#define IR_STATE_AFTER_MARK  3
#define IR_STATE_AFTER_REPEAT 4

mBlockIR* mBlockIR::_instance = NULL;

mBlockIR::mBlockIR(uint8_t pin) {
  _pin = pin;
  _lastLevel = HIGH;
  _lastEdge = 0;
  _state = IR_STATE_IDLE;
  _bits = 0;
  _frame = 0;
  _lastCode = 0;
  _hasNew = 0;
  _repeat = 0;
}

void mBlockIR::begin() {
  _instance = this;

  pinMode(_pin, INPUT_PULLUP);

  uint8_t oldSREG = SREG;
  cli();
  _lastLevel = digitalRead(_pin);
  _lastEdge = micros();
  _state = IR_STATE_IDLE;
  _bits = 0;
  _frame = 0;
  _lastCode = 0;
  _hasNew = 0;
  _repeat = 0;
  SREG = oldSREG;

  // مقاطعة خارجية على الطرف 2 (INT0) أو 3 (INT1) فقط.
  // في نواة Arduino القديمة التي يستخدمها mBlock يأخذ attachInterrupt رقم
  // المقاطعة لا رقم الطرف، لذلك 2→0 و3→1. لا نستخدم PCINT لأنها تتعارض
  // مع SoftwareSerial.
  if (_pin == 2) {
    attachInterrupt(0, mBlockIR::handleInterrupt, CHANGE);
  } else if (_pin == 3) {
    attachInterrupt(1, mBlockIR::handleInterrupt, CHANGE);
  }
}

bool mBlockIR::buttonPressed() {
  bool pressed = false;
  uint8_t oldSREG = SREG;
  cli();
  if (_hasNew) {
    _hasNew = 0;
    pressed = true;
  }
  SREG = oldSREG;
  return pressed;
}

unsigned long mBlockIR::readCode() {
  uint32_t code;
  uint8_t oldSREG = SREG;
  cli();
  code = _lastCode;
  SREG = oldSREG;
  return code;
}

String mBlockIR::readCodeHex() {
  uint32_t code = readCode();
  if (code == 0) {
    return String("");
  }

  // 8 خانات ست عشرية بأحرف كبيرة، مع أصفار بادئة.
  char buffer[9];
  for (int8_t i = 7; i >= 0; i--) {
    uint8_t nibble = (uint8_t)(code & 0x0F);
    buffer[i] = (nibble < 10) ? (char)('0' + nibble) : (char)('A' + nibble - 10);
    code >>= 4;
  }
  buffer[8] = '\0';
  return String(buffer);
}

bool mBlockIR::codeMatches(const String &expected) {
  uint32_t code = readCode();
  if (code == 0) {
    // لا يوجد كود بعد.
    return false;
  }

  // نحوّل النص إلى رقم مع تجاهل المسافات والفواصل وحالة الأحرف.
  uint32_t wanted = 0;
  uint8_t digits = 0;
  for (unsigned int i = 0; i < expected.length(); i++) {
    char c = expected.charAt(i);
    if (c == ' ' || c == ':' || c == '-' || c == '_' || c == '.' ||
        c == 'x' || c == 'X') {
      continue;
    }

    uint8_t value;
    if (c >= '0' && c <= '9') {
      value = (uint8_t)(c - '0');
    } else if (c >= 'a' && c <= 'f') {
      value = (uint8_t)(c - 'a' + 10);
    } else if (c >= 'A' && c <= 'F') {
      value = (uint8_t)(c - 'A' + 10);
    } else {
      return false;  // حرف غير صالح.
    }

    if (digits >= 8) {
      return false;  // أطول من 8 خانات ست عشرية.
    }

    wanted = (wanted << 4) | value;
    digits++;
  }

  if (digits == 0) {
    return false;
  }

  return wanted == code;
}

bool mBlockIR::repeat() {
  bool value;
  uint8_t oldSREG = SREG;
  cli();
  value = (_repeat != 0);
  SREG = oldSREG;
  return value;
}

void mBlockIR::clear() {
  uint8_t oldSREG = SREG;
  cli();
  _lastCode = 0;
  _hasNew = 0;
  _repeat = 0;
  _bits = 0;
  _frame = 0;
  _state = IR_STATE_IDLE;
  SREG = oldSREG;
}

void mBlockIR::handleInterrupt() {
  if (_instance != NULL) {
    _instance->onPinChange();
  }
}

void mBlockIR::onPinChange() {
  uint8_t level = (uint8_t)digitalRead(_pin);
  uint32_t now = micros();
  uint32_t width = now - _lastEdge;
  uint8_t previous = _lastLevel;

  _lastEdge = now;
  _lastLevel = level;

  if (level == previous) {
    return;
  }

  if (level == LOW) {
    // انتهى فراغ (HIGH) وبدأت نبضة (LOW).
    onSpace(width);
  } else {
    // انتهت نبضة (LOW) وبدأ فراغ (HIGH).
    onMark(width);
  }
}

void mBlockIR::onMark(uint32_t width) {
  // نبضة البداية أو نبضة إعادة الإرسال ~9ms.
  if (width >= IR_LEADER_MARK_MIN && width <= IR_LEADER_MARK_MAX) {
    _state = IR_STATE_AFTER_LEAD;
    _bits = 0;
    _frame = 0;
    return;
  }

  // نبضة بت ~560us.
  if (width >= IR_BIT_MARK_MIN && width <= IR_BIT_MARK_MAX) {
    if (_state == IR_STATE_READY_BITS) {
      _state = IR_STATE_AFTER_MARK;
      return;
    }
    if (_state == IR_STATE_AFTER_REPEAT) {
      _state = IR_STATE_IDLE;
      return;
    }
  }

  // أي شيء آخر يعني أن الإطار غير صالح.
  resetFrame();
}

void mBlockIR::onSpace(uint32_t width) {
  if (_state == IR_STATE_AFTER_LEAD) {
    if (width >= IR_LEADER_SPACE_MIN && width <= IR_LEADER_SPACE_MAX) {
      // فراغ البداية 4.5ms: الإطار الجديد سيبدأ الآن.
      _state = IR_STATE_READY_BITS;
      return;
    }
    if (width >= IR_REPEAT_SPACE_MIN && width <= IR_REPEAT_SPACE_MAX) {
      // فراغ إعادة الإرسال 2.25ms: الزر ما زال مضغوطًا.
      _hasNew = 1;
      _repeat = 1;
      _state = IR_STATE_AFTER_REPEAT;
      return;
    }
  }

  if (_state == IR_STATE_AFTER_MARK) {
    if (width <= IR_BIT_ZERO_MAX) {
      appendBit(0);
      return;
    }
    if (width >= IR_BIT_ONE_MIN && width <= IR_BIT_ONE_MAX) {
      appendBit(1);
      return;
    }
  }

  resetFrame();
}

void mBlockIR::appendBit(uint8_t bit) {
  _frame >>= 1;
  if (bit) {
    _frame |= 0x80000000UL;
  }
  _bits++;

  if (_bits == 32) {
    _lastCode = _frame;
    _hasNew = 1;
    _repeat = 0;
    _bits = 0;
    _frame = 0;
    _state = IR_STATE_IDLE;
    return;
  }

  _state = IR_STATE_READY_BITS;
}

void mBlockIR::resetFrame() {
  _state = IR_STATE_IDLE;
  _bits = 0;
  _frame = 0;
}
