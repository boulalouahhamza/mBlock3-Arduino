# Arduino for mBlock 3 — حزمة الإضافات

حزمة جاهزة للنقل إلى أي جهاز. تحتوي خمس إضافات لـ **mBlock 3** مع مكتبات Arduino
الخاصة بها والأمثلة والرسوم التوضيحية. **التثبيت يدوي بالكامل** — لا يوجد أي سكربت
أو ملف تنفيذي، فقط نسخ ولصق.

| الإضافة | الوظيفة | المكتبة |
|---|---|---|
| **DHT Sensor** | قراءة الحرارة والرطوبة من DHT11 / DHT22 | `mBlockDHT` |
| **LCD with I2C Interface Plus** | التحكم بشاشات LCD 1602 / 2004 عبر I2C | `NewliquidCrystal` |
| **RFID RC522** | قراءة بطاقات RFID 13.56MHz عبر SPI | `mBlockRFID` |
| **IR Receiver VS1838B** | استقبال أوامر ريموت NEC عبر مستقبل 38kHz | `mBlockIR` |
| **Bluetooth HC-05** | إرسال واستقبال نصوص من الهاتف عبر HC-05 / HC-06 (SPP) | `mBlockBluetooth` |

---

## 1) محتويات الحزمة

```
mBlock3-Arduino/
├── README.md                  ← هذا الملف
├── extensions/                ← الإضافات الخمس (هذه هي التي تُنسخ إلى mBlock)
│   ├── DHT Sensor/
│   ├── LCD with I2C Interface Plus/
│   ├── RFID RC522/
│   ├── IR Receiver VS1838B/
│   └── Bluetooth HC-05/
├── libraries/                 ← مكتبات Arduino (تُنسخ إلى مجلد Arduino داخل mBlock)
│   ├── mBlockDHT/
│   ├── NewliquidCrystal/
│   ├── mBlockRFID/
│   ├── mBlockIR/
│   └── mBlockBluetooth/
├── examples/                  ← 15 مثالًا جاهزًا (.sb2)
│   ├── DHT/                   ← DHT_1_Basic، DHT_2_Hot_or_Cold، DHT_3_Humidity_Meter
│   ├── LCD_I2C/               ← LCD_1_Hello_World، LCD_2_My_Name، LCD_3_Countdown
│   ├── RFID/                  ← RFID_1_Show_UID، RFID_2_Access_Control، RFID_3_Two_Friends
│   ├── IR/                    ← IR_1_Show_Code، IR_2_One_Button، IR_3_Two_Buttons
│   └── Bluetooth/             ← BT_1_Hello_Phone، BT_2_LED_Command، BT_3_Echo
└── diagrams/                  ← 5 رسوم توصيل تُفتح ببرنامج Fritzing
    ├── DHT11_LCD_I2C.fzz
    ├── LCD_I2C_16x2.fzz
    ├── RFID_RC522.fzz
    ├── IR_VS1838B.fzz
    └── Bluetooth HC-05.fzz
```

> كل إضافة تحتوي أيضًا نسخة من مكتبتها في `arduino/libraries/` حتى تعمل وحدها،
> لكن **يجب** نسخ مجلد `libraries/` إلى مسار Arduino كما في الخطوة 3.2.

---

## 2) المتطلبات

- **mBlock 3** مثبت على Windows (المسار الافتراضي `C:\Program Files (x86)\mBlock`).
- **Arduino Uno** أو لوحة AVR متوافقة (Nano / Mega مع تعديل أرقام المنافذ).
- كابل USB وتعريف المنفذ (غالبًا **CH340**).
- صلاحيات مسؤول **مرة واحدة** للنسخ داخل `C:\Program Files (x86)\mBlock`.
- لا تحتاج إنترنت ولا برامج إضافية على الحاسوب (تطبيق طرفية بلوتوث على الهاتف
  مطلوب فقط لتجريب أمثلة Bluetooth، مثل Serial Bluetooth Terminal).

---

## 3) التثبيت اليدوي

> أغلق **mBlock 3** بالكامل قبل البدء (تأكد من عدم وجود `mBlock.exe` في مدير المهام).
> إذا كانت الحزمة على فلاشة USB، انسخها إلى سطح المكتب أولًا ثم ابدأ التثبيت.

### 3.1) تثبيت الإضافات الخمس

الخطوة الوحيدة: **انسخ المجلدات الخمس** من `extensions/` إلى مجلد مكتبات mBlock:

```
%APPDATA%\com.makeblock.Scratch3.4.12\Local Store\mBlock\libraries\
```

- افتح مستكشف الملفات، واكتب المسار أعلاه في شريط العنوان (أو الصق
  `%APPDATA%` واضغط Enter ثم اتبع `com.makeblock.Scratch3.4.12\Local Store\mBlock\libraries`).
- إن لم يوجد مجلد بإصدار `Scratch3.4.12` بالضبط، ابحث عن أقرب اسم مثل
  `com.makeblock.Scratch3.4.*` واستخدمه.

بعد النسخ يجب أن يصبح الشكل:

```
...\Local Store\mBlock\libraries\
├── DHT Sensor\
│   ├── DHT.s2e
│   ├── js\DHT.js
│   └── arduino\libraries\mBlockDHT\
├── LCD with I2C Interface Plus\
│   ├── LCDI2C.s2e
│   ├── js\LCDI2C.js
│   └── arduino\libraries\NewliquidCrystal\
├── RFID RC522\
│   ├── RFID.s2e
│   ├── js\RFID.js
│   └── arduino\libraries\mBlockRFID\
├── IR Receiver VS1838B\
│   ├── IRReceiver.s2e
│   ├── js\IRReceiver.js
│   └── arduino\libraries\mBlockIR\
└── Bluetooth HC-05\
    ├── BluetoothHC05.s2e
    ├── js\BluetoothHC05.js
    └── arduino\libraries\mBlockBluetooth\
```

> **مهم:** لا تُغيّر أسماء المجلدات. اسم المجلد يجب أن يطابق `extensionName`
> الموجود داخل ملف `.s2e`، وإلا لن تظهر الإضافة في mBlock.

### 3.2) تثبيت مكتبات Arduino

مترجم mBlock لا يقرأ مجلد `extensions`، بل يقرأ مكتبات Arduino من مجلدات التثبيت.
انسخ المكتبات الخمس من `libraries/` إلى **المسار الرئيسي** التالي:

**أ) المسار الرئيسي (مطلوب — يحتاج صلاحيات مسؤول):**

```
C:\Program Files (x86)\mBlock\Arduino\portable\sketchbook\libraries\
```

هذا هو المجلد الذي يقرأه mBlock فعلًا: إعداد `sketchbook.path=sketchbook` داخل
`Arduino\portable\preferences.txt` يجعل mBlock يبحث عن المكتبات في
`Arduino\portable\sketchbook\libraries\`. وهو أيضًا المسار الذي يأخذ الأولوية عند
وجود مكتبة بنفس الاسم في مكان آخر.

**ب) مسار إضافي (مستحسن — يحتاج صلاحيات مسؤول):**

```
C:\Program Files (x86)\mBlock\Arduino\libraries\
```

مجلد المكتبات المدمجة في Arduino IDE؛ النسخ إليه لا يضر ويعمل مع بعض إصدارات mBlock.

النتيجة في كل مسار:

```
...\libraries\
├── mBlockDHT\
├── NewliquidCrystal\
├── mBlockRFID\
├── mBlockIR\
└── mBlockBluetooth\
```

طريقة النسخ: افتح المسار — سيطلب Windows موافقة المسؤول، اضغط **Continue**.
ثم اسحب المجلدات الخمس من `libraries/` وأفلتها هناك.

> **تنبيه:** إن وُجد مجلد مكتبة قديم بنفس الاسم (مثل `mBlockRFID`) فاحذفه أولًا
> ثم انسخ الجديد، حتى لا تبقى نسخة قديمة تأخذ الأولوية.

**ج) اختياري — إن كنت تستخدم Arduino IDE على نفس الجهاز:**

```
%USERPROFILE%\Documents\Arduino\libraries\
```

### 3.3) التحقق من التثبيت

1. افتح **mBlock 3**.
2. افتح قائمة **Extensions** — يجب أن تجد الإضافات الخمس بأسمائها:
   `DHT Sensor` و `LCD with I2C Interface Plus` و `RFID RC522` و
   `IR Receiver VS1838B` و `Bluetooth HC-05`.
3. اسحب أي كتلة منها إلى منطقة البرمجة — إذا ظهرت الكتلة فهذا يعني أن ملف `.s2e` سليم.
4. جرّب **Upload** لأحد الأمثلة؛ إذا نجحت الترجمة فالمكتبات في المكان الصحيح.

---

## 4) تشغيل الأمثلة

1. في mBlock: **File → Open** واختر ملف `.sb2` من مجلد `examples/`.
2. وصّل اللوحة واختر **Board: Arduino Uno** و**Port** الصحيح (مثل `COM6`).
3. اضغط **Upload**.

| المثال | الوصف | المنافذ |
|---|---|---|
| `examples/LCD_I2C/LCD_1_Hello_World.sb2` | يكتب `Hello World!` على الشاشة | LCD على I2C |
| `examples/LCD_I2C/LCD_2_My_Name.sb2` | يكتب اسمك في مكان محدد | LCD على I2C |
| `examples/LCD_I2C/LCD_3_Countdown.sb2` | عدّ تنازلي 3 → 2 → 1 → GO!! | LCD على I2C |
| `examples/DHT/DHT_1_Basic.sb2` | يعرض الحرارة والرطوبة | DHT على D2، LCD على I2C |
| `examples/DHT/DHT_2_Hot_or_Cold.sb2` | كلمات حسب درجة الحرارة | DHT على D2، LCD على I2C |
| `examples/DHT/DHT_3_Humidity_Meter.sb2` | مقياس رطوبة: WET / OK / DRY | DHT على D2، LCD على I2C |
| `examples/RFID/RFID_1_Show_UID.sb2` | يعرض UID البطاقة | RC522: SS=10، RST=9 |
| `examples/RFID/RFID_2_Access_Control.sb2` | تحكم بالدخول: مسموح / مرفوض | RC522: SS=10، RST=9 |
| `examples/RFID/RFID_3_Two_Friends.sb2` | يحيّي صاحب كل بطاقة باسمه | RC522: SS=10، RST=9 |
| `examples/IR/IR_1_Show_Code.sb2` | يعرض كود كل زر على الشاشة | VS1838B على D3، LCD على I2C |
| `examples/IR/IR_2_One_Button.sb2` | يميّز زرًا واحدًا عن بقية الأزرار | VS1838B على D3، LCD على I2C |
| `examples/IR/IR_3_Two_Buttons.sb2` | يميّز زرين ويكتب اسم كل واحد | VS1838B على D3، LCD على I2C |
| `examples/Bluetooth/BT_1_Hello_Phone.sb2` | يرسل `Hello from Arduino!` للهاتف كل ثانيتين | HC-05: RX=10، TX=11 |
| `examples/Bluetooth/BT_2_LED_Command.sb2` | يضيء LED D13 بأمر `on` ويطفئه بأمر `off` | HC-05: RX=10، TX=11، LED على D13 |
| `examples/Bluetooth/BT_3_Echo.sb2` | يعيد للهاتف أي نص يرسله (`You said: ...`) | HC-05: RX=10، TX=11 |

> ابدأ دائمًا بملف `_1_` في كل إضافة، واستخدم نفس أرقام المنافذ الموجودة في
> المثال أو عدّلها لتطابق توصيلك.

> الأمثلة تحتوي تعريفات الإضافات داخلها، لذلك تُفتح وتعمل مباشرة بعد التثبيت.

### ضبط بطاقة الدخول في مثال التحكم بالدخول

1. ارفع `RFID_1_Show_UID.sb2` أولًا واقرأ UID الـ tag من الشاشة.
2. افتح `RFID_2_Access_Control.sb2` وابحث عن الكتلة
   `RFID UID on SS pin [10] matches [B7 CB FF 63]`، ثم استبدل النص بـ UID
   الـ tag (أحرف كبيرة ومسافة بين كل بايتين، مثل `B7 CB FF 63`).
3. أعد الرفع.

القيمة الافتراضية `B7 CB FF 63` مجرد مثال. البطاقة العادية تُرفض دائمًا،
ويُسمح فقط للـ tag الذي تضع UID الخاص به.

> في `RFID_3_Two_Friends.sb2` توجد كتلتا مطابقة: ضع UID الأول في الأولى
> والثاني في الثانية.
>
> كتلة المطابقة تتجاهل المسافات وحالة الأحرف، فـ `B7CBFF63` تساوي `B7 CB FF 63`.
>
> **الأمثلة لا تستخدم متغيرات، وهذا مقصود:** متغيرات mBlock من نوع `double`
> فقط، ووضع نص فيها (مثل UID) يسبب خطأ ترجمة و«Upload failed».

### ضبط أكواد الريموت في أمثلة IR

1. ارفع `IR_1_Show_Code.sb2` أولًا واضغط الزر المطلوب في الريموت.
2. اكتب الكود الظاهر على الشاشة، مثل `00FFA25D`.
3. في `IR_2_One_Button.sb2` عدّل الكتلة
   `IR code on pin [3] matches [00FFA25D]` وضع كود زرّك.
4. في `IR_3_Two_Buttons.sb2` توجد كتلتا مطابقة: كود الزر الأول في الأولى
   والثاني في الثانية.

كتلة المطابقة تقبل الكود بأي صيغة: `00FFA25D` أو `00ffa25d` أو `FFA25D`.

> **الأمثلة لا تستخدم متغيرات، وهذا مقصود:** متغيرات mBlock من نوع `double`
> فقط، ووضع نص فيها (مثل كود الريموت) يسبب خطأ ترجمة و«Upload failed».

### تجريب أمثلة البلوتوث من الهاتف

1. بعد الرفع، فعّل البلوتوث في الهاتف وابحث عن جهاز اسمه `HC-05` (أو `HC-06`)
   واقترنه بكلمة المرور الافتراضية `1234` أو `0000`.
2. ثبّت تطبيق طرفية تسلسلية عبر بلوتوث مثل **Serial Bluetooth Terminal**
   (أندرويد) واتصل بجهاز HC-05.
3. `BT_1_Hello_Phone.sb2`: ستظهر رسالة كل ثانيتين بلا إرسال منك.
4. `BT_3_Echo.sb2`: اكتب أي كلمة واضغط Send — يعيدها Arduino بصيغة
   `You said: <كلمتك>`.
5. `BT_2_LED_Command.sb2`: أرسل `on` ليضيء LED المدمج على D13، أو `off`
   ليطفئه. الأحرف الكبيرة والصغيرة والمسافات لا تهم (`ON` = `on`).

---

## 5) التوصيلات

### DHT11 / DHT22

| المستشعر | Arduino Uno |
|---|---|
| VCC | 5V |
| DATA | D2 (أو أي منفذ رقمي غير D0/D1) |
| GND | GND |

وحدات DHT الجاهزة تحتوي مقاومة Pull-up. للمستشعر الخام أضف **10kΩ** بين DATA و VCC.

### LCD عبر I2C

| LCD I2C | Arduino Uno |
|---|---|
| GND | GND |
| VCC | 5V |
| SDA | A4 |
| SCL | A5 |

العنوان الافتراضي `0x27`. إذا كانت الشاشة مضيئة بلا نص جرّب `0x3F`.

### RFID RC522

| RC522 | Arduino Uno |
|---|---|
| SDA / SS | D10 |
| SCK | D13 |
| MOSI | D11 |
| MISO | D12 |
| RST | D9 |
| VCC | **3.3V فقط** |
| GND | GND |

> **تحذير:** توصيل RC522 على 5V قد يتلفه نهائيًا. استخدم 3.3V.
> البطاقات 125kHz (مثل EM4100) لا تعمل مع RC522؛ المطلوب بطاقة **13.56MHz**
> مثل MIFARE Classic.

### VS1838B IR Receiver

| VS1838B | Arduino Uno |
|---|---|
| OUT / DATA | D3 |
| GND | GND |
| VCC | 5V |

- المستقبل يعمل على **D2 أو D3 فقط** (INT0/INT1)، و **D3** هو الافتراضي.
- لا تستخدم `D0` و`D1` (Serial).
- إذا استخدمت RC522 في نفس المشروع، تجنّب `D13` (SCK) و`D10` (SS).
- بعض الوحدات تعكس ترتيب الأطراف؛ اقرأ الطباعة على اللوحة قبل التوصيل.

### Bluetooth HC-05 / HC-06

| HC-05 | Arduino Uno |
|---|---|
| TXD | D10 (RX) |
| RXD | D11 (TX) |
| VCC | 5V (على معظم الوحدات الجاهزة) |
| GND | GND |

- قاعدة الربط: **TXD في الوحدة → RX في Arduino، و RXD في الوحدة → TX في Arduino**
  (التقاطع، وليس TX→TX).
- المنافذ الافتراضية في الأمثلة: **RX = 10** و **TX = 11**، والسرعة **9600**.
- بعض الوحدات الخام تعمل بمنطق 3.3V على RXD؛ إن لم تستجب الوحدة للأوامر
  أضف مفرق جهد (مقاومتان 1kΩ و 2kΩ) بين D11 و RXD.
- الوحدة تستخدم **SoftwareSerial** على منفذين رقميين، لذلك لا تتعارض مع منفذ
  USB (D0/D1) المستخدم في الرفع، ويمكنك اختيار أي منفذ رقمي من القائمة.

الرسوم التفصيلية في مجلد `diagrams/` وتُفتح جميعها ببرنامج **Fritzing**:

- `diagrams/DHT11_LCD_I2C.fzz`
- `diagrams/LCD_I2C_16x2.fzz`
- `diagrams/RFID_RC522.fzz`
- `diagrams/IR_VS1838B.fzz`
- `diagrams/Bluetooth HC-05.fzz`

---

## 6) الكتل المتاحة

### DHT Sensor

| الكتلة | الوظيفة |
|---|---|
| `Setup DHT sensor on pin [2] type [DHT11]` | تهيئة — مرة واحدة في البداية |
| `Read temperature from DHT on pin [2] (°C)` | الحرارة |
| `Read humidity from DHT on pin [2] (%)` | الرطوبة |

استخدم نفس رقم المنفذ في كتل الإعداد والقراءة.

### LCD with I2C Interface Plus

| الكتلة | الوظيفة |
|---|---|
| `Set: LCD at [0x27] has [2] lines and [16] cols` | تهيئة — مرة واحدة |
| `LCD at [0x27] Line [1] Col [1] Show [ ... ]` | كتابة نص |
| `Clear LCD at [0x27] and Line [1] Col [1] Show [ ... ]` | مسح ثم كتابة |
| `Turn [ON/OFF] the backlight of LCD at [0x27]` | الإضاءة الخلفية |
| `Clear LCD at [0x27]` | مسح الشاشة |
| `LCD at [0x27] [show cursor / hide cursor / show blink / hide blink]` | المؤشر |

### RFID RC522

| الكتلة | الوظيفة |
|---|---|
| `Setup RFID RC522 SS pin [10] RST pin [9]` | تهيئة — مرة واحدة |
| `RFID card present on SS pin [10]` | هل توجد بطاقة؟ |
| `Read RFID UID (hex) on SS pin [10]` | UID كنص، مثل `B7 CB FF 63` |
| `Read RFID UID (number) on SS pin [10]` | UID كرقم |
| `RFID UID on SS pin [10] matches [B7 CB FF 63]` | هل UID الحالي يطابق نصًا معينًا؟ |
| `Halt RFID card on SS pin [10]` | إيقاف البطاقة الحالية |

> نتيجة القراءة تُخزَّن مؤقتًا لنفس البطاقة، لذلك يمكن قراءة UID ثم مقارنته
> في نفس المرور بدون فقدان البطاقة.

### IR Receiver VS1838B

| الكتلة | الوظيفة |
|---|---|
| `Setup IR receiver on pin [3]` | تهيئة — مرة واحدة في البداية |
| `IR button pressed on pin [3]` | هل وصل كود جديد أو إعادة إرسال؟ |
| `IR code (hex) on pin [3]` | آخر كود كنص، مثل `00FFA25D` |
| `IR code (number) on pin [3]` | آخر كود كرقم |
| `IR code on pin [3] matches [00FFA25D]` | هل آخر كود يطابق نصًا معينًا؟ |
| `Clear IR code on pin [3]` | مسح الكود المخزّن |

> المكتبة تفكّ ترميز **NEC** فقط، وتستخدم المقاطعة الخارجية **INT0/INT1**
> (الطرفان D2 وD3). استخدم نفس المنفذ في كل الكتل.

### Bluetooth HC-05

| الكتلة | الوظيفة |
|---|---|
| `Setup Bluetooth RX pin [10] TX pin [11] baud [9600]` | تهيئة — مرة واحدة في البداية |
| `Bluetooth send text [Hello]` | إرسال نص بدون سطر جديد |
| `Bluetooth send line [Hello!]` | إرسال نص مع سطر جديد (الأسهل للهاتف) |
| `Bluetooth message ready` | صحيح مرة واحدة عند وصول رسالة كاملة جديدة |
| `Bluetooth message` | نص آخر رسالة وصلت |
| `Bluetooth message matches [on]` | هل آخر رسالة تطابق نصًا معينًا؟ |
| `Bluetooth message as number` | آخر رسالة كرقم صحيح (0 إذا لم تكن رقمًا) |
| `Clear Bluetooth message` | مسح الرسالة المخزّنة |

> استخدم نفس المنفذين في كل الكتل. الرسالة تُعتبر كاملة عند وصول سطر جديد
> `\n` أو `\r`، أو عند توقف الحروف 50 مللي ثانية. أقصى طول للرسالة 48 حرفًا.
> مطابقة النص تتجاهل المسافات وحالة الأحرف: `ON` تساوي `on`.

---

## 7) حل المشاكل

| المشكلة | السبب | الحل |
|---|---|---|
| `mBlockDHT.h: No such file` | المكتبة ليست في مسار Arduino | انسخ `libraries\mBlockDHT` إلى `mBlock\Arduino\libraries\` |
| `LiquidCrystal_I2C.h: No such file` | نفس السبب | انسخ `NewliquidCrystal` إلى نفس المسار |
| `SPI.h: No such file` | نسخة إضافة قديمة | استخدم مجلد `extensions` من هذه الحزمة |
| `Multiple libraries were found...` | المكتبة موجودة في أكثر من مسار | تحذير غير ضار، تجاهله |
| الإضافة لا تظهر في mBlock | اسم المجلد غير مطابق | أعد النسخ باسمه الأصلي دون تغيير |
| الشاشة مضيئة بلا نص | التباين أو العنوان | أدر مقياس التباين، أو غيّر `0x27` إلى `0x3F` |
| حروف غريبة على الشاشة | SDA/SCL غير ثابت | راجع الأسلاك واللحام |
| قيمة `-999` من DHT | فشل قراءة المستشعر | راجع VCC/GND/DATA وأضف مقاومة 10kΩ |
| الرفع ناجح لكن لا تُقرأ البطاقة | تغذية أو توصيل | تأكد من 3.3V ومن MISO/MOSI/SCK/SS |
| البطاقة تعمل مرة واحدة | طبيعي بعد `Halt` | أبعِد البطاقة ثم قرّبها مجددًا |
| لا يظهر أي كود IR | الريموت ليس NEC أو ترتيب الأطراف معكوس | جرّب ريموت NEC آخر وراجع OUT/GND/VCC |
| كود IR يتغير عشوائيًا | تشويش أو سلك طويل | قصّر سلك OUT وأبعِد المستقبل عن الإضاءة |
| الهاتف لا يجد HC-05 | البلوتوث مطفأ أو الوحدة غير مُغذّاة | راجع VCC/GND وتأكد من وميض LED في الوحدة |
| الاقتران يطلب PIN | كلمة مرور الوحدة | جرّب `1234` ثم `0000` |
| متصل لكن لا رسائل | RX/TX معكوسة أو سرعة خاطئة | اعكس D10/D11 وتأكد من السرعة 9600 |
| رسائل مشوّهة أو مكررة | سرعة أو تشويش | ثبّت 9600 وقصّر الأسلاك وتجنّب منافذ PWM المشغولة |
| الرسالة مقطوعة | أطول من 48 حرفًا | أرسل نصوصًا أقصر أو قسّمها لرسائل |
| المنفذ غير ظاهر | تعريف CH340 | ثبّت تعريف CH340 وأعد توصيل الكابل |

---

## 8) ملاحظات مهمة

- شاشة LCD النصية (HD44780) **لا تدعم العربية**. استخدم رسائل إنجليزية:
  `Access granted` = يسمح بالدخول، `Access denied` = لا يحق الدخول.
- مكتبة DHT تخزّن القراءة لمدة **ثانيتين**، لذلك قراءة الحرارة ثم الرطوبة تعمل
  دون فشل. القيمة `-999` تعني تعذّرت القراءة.
- mBlock 3 يترجم بـ `gnu++98`، ومكتبة `MFRC522` المرفقة معدّلة لتتوافق معه.
- مستقبل IR يفكّ ترميز **NEC** فقط. ريموتات Sony SIRC و RC5 و RC6 لا تعمل.
- مكتبة IR تستخدم المقاطعة الخارجية **INT0/INT1** (الطرفان D2 وD3) وليست
  PCINT، لذلك لا تتعارض مع `SoftwareSerial` التي يضيفها mBlock لكل مشروع.
  لا تجمعها مع مكتبة أخرى تستخدم INT0/INT1 في نفس الوقت.
- مكتبة البلوتوث تعمل عبر **SoftwareSerial** على منفذين رقميين من اختيارك،
  وتجمع الرسائل حتى نهاية السطر (أو سكون 50 مللي ثانية). طول الرسالة الأقصى
  **48 حرفًا**، والرسالة الأخيرة تبقى محفوظة حتى تصل رسالة جديدة أو تُمسح.
- عند تحديث الحزمة على جهاز سابق: احذف مجلد الإضافة القديم أولًا ثم انسخ الجديد.
- التثبيت على أجهزة متعددة: انسخ مجلدات `extensions` و`libraries` نفسها في كل جهاز؛
  المسارات هي نفسها دائمًا ما دام إصدار mBlock متشابهًا.
