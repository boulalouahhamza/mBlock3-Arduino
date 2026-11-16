/*!
 *  @file mBlockDHT.h
 *
 *  مكتبة DHT11 / DHT22 مخصصة لإضافة mBlock 3.
 *  لا تعتمد على أي مكتبة خارجية سوى Arduino.h.
 *
 *  القراءة مخزنة مؤقتًا لمدة ثانيتين، لذلك يمكن استدعاء
 *  readTemperature() و readHumidity() تباعًا دون إعادة قراءة المستشعر.
 */

#ifndef MBLOCK_DHT_H
#define MBLOCK_DHT_H

#include "Arduino.h"

// أنواع المستشعرات
#define DHT_TYPE_11 11
#define DHT_TYPE_22 22

class mBlockDHT {
 public:
  mBlockDHT(uint8_t pin, uint8_t type);
  void begin();

  float readTemperature();  // درجة الحرارة بالسيليزيوس
  float readHumidity();     // الرطوبة بالنسبة المئوية
  bool readRaw();           // قراءة خام (true إذا نجحت) وتحديث القيم المخزنة

 private:
  bool _readSensor();       // قراءة 40 بت من المستشعر
  bool _update();           // قراءة جديدة فقط إذا انتهت مدة التخزين المؤقت
  void _decode();           // تحويل البايتات الخام إلى قيم فيزيائية

  uint8_t _pin;
  uint8_t _type;
  uint8_t _data[5];
  uint32_t _lastRead;
  float _lastTemperature;
  float _lastHumidity;
  bool _hasReading;
};

#endif
