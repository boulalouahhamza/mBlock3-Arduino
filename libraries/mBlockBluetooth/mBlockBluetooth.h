/*!
 *  @file mBlockBluetooth.h
 *
 *  مكتبة البلوتوث (HC-05 / HC-06) لإضافة mBlock 3.
 *
 *  تعمل الوحدة عبر منفذين رقميين باستخدام SoftwareSerial، لذلك لا تتعارض
 *  مع منفذ USB (D0/D1) المستخدم في الرفع والمراقب التسلسلي.
 *
 *  الفكرة:
 *   - الهاتف يرسل نصًا ينتهي بسطر جديد (Newline).
 *   - المكتبة تجمع الحروف في مخزن مؤقت وتعتبر الرسالة كاملة عند وصول
 *     '\n' أو '\r'، أو عند توقف الحروف لمدة 50 مللي ثانية.
 *   - آخر رسالة تبقى محفوظة حتى تصل رسالة جديدة أو يتم مسحها.
 *
 *  ملاحظات:
 *   - استخدم نفس المنفذين في كل الكتل.
 *   - D10 للاستقبال (RXD على الوحدة) و D11 للإرسال (TXD على الوحدة).
 *   - سرعة HC-05/HC-06 الافتراضية 9600.
 */

#ifndef MBLOCK_BLUETOOTH_H
#define MBLOCK_BLUETOOTH_H

#include "Arduino.h"
#include <SoftwareSerial.h>

// أقصى طول للرسالة الواحدة (بما فيها حرف النهاية).
#define MBLOCK_BT_BUFFER 48

// إذا توقفت الحروف هذه المدة تُعتبر الرسالة كاملة حتى بدون سطر جديد.
#define MBLOCK_BT_TIMEOUT_MS 50

class mBlockBluetooth {
 public:
  // rxPin يتصل بـ TXD في الوحدة، وtxPin يتصل بـ RXD في الوحدة.
  mBlockBluetooth(uint8_t rxPin, uint8_t txPin);

  // تهيئة المنفذ. استدعها مرة واحدة قبل أي كتلة أخرى.
  void begin(long baud);

  // إرسال نص بدون سطر جديد.
  void sendText(const String &text);

  // إرسال نص مع سطر جديد (الأسهل للقراءة على الهاتف).
  void sendLine(const String &text);

  // صحيح مرة واحدة عند وصول رسالة كاملة جديدة.
  // استدعاؤها يستهلك إشارة "رسالة جديدة" لكنها لا تمسح نص الرسالة.
  bool messageReady();

  // نص آخر رسالة كاملة (نص فارغ إذا لم تصل رسالة).
  String readMessage();

  // صحيح إذا كان نص آخر رسالة يطابق النص المعطى.
  // يتجاهل المسافات الزائدة وحالة الأحرف: "ON" تساوي "on".
  bool messageMatches(const String &expected);

  // آخر رسالة كرقم صحيح (0 إذا لم تكن رقمًا).
  long readNumber();

  // مسح الرسالة المخزّنة.
  void clearMessage();

 private:
  void poll();
  void finishMessage();

  SoftwareSerial _serial;
  uint8_t _rxPin;
  uint8_t _txPin;

  char _buffer[MBLOCK_BT_BUFFER];
  uint8_t _length;

  char _message[MBLOCK_BT_BUFFER];
  bool _hasMessage;
  bool _newMessage;
  bool _delivered;
  unsigned long _lastByteMs;
};

#endif
