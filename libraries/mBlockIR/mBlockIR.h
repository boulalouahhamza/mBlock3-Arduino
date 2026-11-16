/*!
 *  @file mBlockIR.h
 *
 *  مكتبة استقبال الأشعة تحت الحمراء لمستقبل VS1838B (38kHz) لإضافة mBlock 3.
 *
 *  تفكّ ترميز بروتوكول NEC وهو البروتوكول المستخدم في معظم أجهزة التحكم
 *  الرخيصة. القراءة تعمل عبر مقاطعة خارجية (INT0/INT1) لذلك لا تفوت
 *  الضغطة حتى لو كان البرنامج مشغولًا بكتل أخرى.
 *
 *  ملاحظة مهمة:
 *   - المستقبل يجب أن يكون على الطرف 2 أو 3 (INT0/INT1).
 *   - لا تستخدم مقاطعة تغيّر الحالة (PCINT) لأنها تتعارض مع مكتبة
 *     SoftwareSerial التي يضيفها mBlock لكل مشروع.
 *   - تُدعم قطعة استقبال واحدة لكل لوحة.
 */

#ifndef MBLOCK_IR_H
#define MBLOCK_IR_H

#include "Arduino.h"

class mBlockIR {
 public:
  explicit mBlockIR(uint8_t pin);

  // تهيئة الطرف وتفعيل المقاطعة. استدعها مرة واحدة قبل أي كتلة أخرى.
  void begin();

  // صحيح إذا وصل كود جديد (أو إعادة إرسال) منذ آخر استدعاء.
  // استدعاؤها يستهلك الحدث ولا يمسح الكود نفسه.
  bool buttonPressed();

  // آخر كود تم فك ترميزه (0 إذا لم يوجد).
  unsigned long readCode();

  // آخر كود بصيغة ست عشرة (8 خانات)، مثل "00FFA25D".
  // ترجع نصًا فارغًا إذا لم يوجد كود.
  String readCodeHex();

  // صحيح إذا كان آخر كود يطابق النص المعطى.
  // يتجاهل المسافات والفواصل وحالة الأحرف ويقبل أقل من 8 خانات
  // (مثل "FFA25D" تعادل "00FFA25D").
  bool codeMatches(const String &expected);

  // صحيح إذا كان الكود الأخير إعادة إرسال (الزر مضغوط باستمرار).
  bool repeat();

  // مسح الكود الحالي.
  void clear();

  // يُستدعى من مقاطعة تغيّر الحالة، لا تستدعها من البرنامج.
  static void handleInterrupt();

 private:
  void onPinChange();
  void onMark(uint32_t width);
  void onSpace(uint32_t width);
  void appendBit(uint8_t bit);
  void resetFrame(void);

  uint8_t _pin;
  volatile uint8_t _lastLevel;
  volatile uint32_t _lastEdge;
  volatile uint8_t _state;
  volatile uint8_t _bits;
  volatile uint32_t _frame;
  volatile uint32_t _lastCode;
  volatile uint8_t _hasNew;
  volatile uint8_t _repeat;

  static mBlockIR* _instance;
};

#endif
