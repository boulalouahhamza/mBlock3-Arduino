/*!
 *  @file mBlockRFID.h
 *
 *  طبقة تبسيط فوق مكتبة MFRC522 (قارئ RFID RC522) لإضافة mBlock 3.
 *  توفر دوال بسيطة: cardPresent / readUID / readUIDNumber / uidMatches / halt.
 *
 *  ملاحظة مهمة:
 *   بعد قراءة البطاقة وإيقافها (Halt) لا يمكن قراءتها مرة أخرى في نفس
 *   المرور، لذلك تُخزَّن آخر UID في الذاكرة وتُستخدم في كل الاستدعاءات
 *   التالية حتى تُكتشف بطاقة جديدة.
 */

#ifndef MBLOCK_RFID_H
#define MBLOCK_RFID_H

#include "Arduino.h"
#include "MFRC522.h"

class mBlockRFID {
 public:
  mBlockRFID(uint8_t ssPin, uint8_t rstPin);

  void begin();                        // تهيئة القارئ
  bool cardPresent();                  // هل توجد بطاقة جديدة؟
  String readUID();                    // قراءة UID كنص سداسي عشري
  unsigned long readUIDNumber();       // قراءة UID كرقم
  bool uidMatches(const String &expected);  // هل UID يطابق نصًا معينًا؟
  void halt();                         // إيقاف البطاقة الحالية

 private:
  bool _readUIDBytes();                // يقرأ البايتات داخليًا
  bool _ensureUID();                   // يقرأ البطاقة إن لم تكن مقروءة
  void _clearUID();                    // يمسح UID المخزَّن

  MFRC522 _rfid;
  bool _cardReady;                     // هل تم اكتشاف بطاقة في آخر فحص؟
  bool _hasUID;                        // هل UID المخزَّن صالح؟
  uint8_t _uidLen;                     // عدد بايتات UID
  uint8_t _uidBuf[10];                 // بايتات UID (حتى 10 بايت)
};

#endif
