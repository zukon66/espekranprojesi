# ESPekran Firmware Skeleton

ESPekran, gelecekte ESP32 tabanli bir haberlesme cihazi ve mini el konsolu olmasi hedeflenen bir firmware prototipidir. Bu repo su anda gercek donanim suruculeri iceren tamamlanmis bir urun degil; katmanli, testlenebilir bir yazilim iskeleti ve yazilim simulesi sunar.

## Proje Durumu
- Son README Sync: 2026-03-29
- Last sync commit: `dcd486b`
- Milestone progress: `4/8 (~50%)`

### En son yapilanlar
- Yapildi: Tek dosyali `main.cpp` yapisi katmanli mimariye ayrildi.
- Yapildi: `app/core`, `ui`, `input`, `transport`, `storage`, `power` ve `board/hal` sinirlari olusturuldu.
- Yapildi: `Display`, `Input`, `RadioTransport`, `Storage`, `Clock` ve `PowerManager` arayuzleri eklendi.
- Yapildi: Host tarafinda calisabilen native test ortami eklendi.
- Yapildi: UI state transitions ve mock radio queue davranisi icin test kaynaklari eklendi.
- Guncellendi: README, yazilimda dogrulanan kisimlari ve donanimda dogrulanmayan kisimlari ayiracak sekilde duzeltildi.

### Su an uzerinde calisilan
- Yazilim mimarisini gercek TFT, touch, NRF24L01, SD ve power entegrasyonuna hazir tutmak.
- Mock adapter'lari gercek driver'lara gecis yapilabilecek sinirlar olarak korumak.

### Sonraki adim
- `Display` arayuzune ilk gercek TFT adapter'ini baglamak.
- `Input` arayuzune D-Pad veya touch icin ilk donanim adapter'ini eklemek.
- `RadioTransport` arayuzune NRF24L01 tabanli ilk surucuyu baglamak.

## Mimari Ozet
Repo iki ana bolume ayrilir:

- `lib/espekran`: Saf C++ cekirdek kod. Arduino ve FreeRTOS bagimliligi yoktur.
- `src`: ESP32/Arduino runtime wiring katmani. Task olusturma, `Serial`, `millis`, pin setup ve demo simulasyon burada kalir.

Temel katmanlar:
- `app/core`: Uygulama durumu ve `AppController`
- `ui`: `DeviceState` -> `UiFrame` sunumu
- `input`: normalize input event modeli
- `transport/radio`: `RadioTransport` arayuzu ve mock queue mantigi
- `storage`: depolama arayuzu
- `power`: power snapshot arayuzu
- `board/hal`: ESP32'ye ozel glue kod

## Implemented and Validated in Software
Asagidaki kisimlar yazilim seviyesinde uygulanmis ve bu makinede build ile dogrulanmistir:

- `AppController` ile ekran gecisleri ve mesaj state yonetimi
- `UiFrame` uretimi ve serial tabanli gorunum modeli
- `MockRadioTransport` ile kapasite sinirli software queue davranisi
- `MockPowerManager` ile software power snapshot simulasyonu
- `platformio run -e esp32dev` ile firmware derlenebilirligi

## Implemented but Hardware-Untested
Asagidaki kisimlar sadece yazilim olarak modellenmistir. Fiziksel cihazda dogrulanmamistir:

- Serial tabanli UI dongusu
- FreeRTOS task ayrimi
- ESP32 pin konfigrasyonu
- Mock radio akisi
- Mock battery / weather / connectivity simulasyonu
- Host-side native test kaynaklari mevcut, ancak bu makinede `gcc/g++` olmadigi icin `platformio test -e native` calistirilamadi

Bu maddeler NRF24L01, TFT, touch, SD veya guc yonetiminin gercek donanimda calistigini kanitlamaz.

## Not Implemented Yet
- Gercek TFT surucusu
- Gercek touch input surucusu
- Gercek NRF24L01 transport entegrasyonu
- Gercek SD card storage entegrasyonu
- Gercek audio cikisi
- Gercek sleep/wake dogrulamasi

## Milestone Checklist
- [x] PlatformIO + ESP32 firmware iskeleti kuruldu
- [x] Katmanli core/controller mimarisi kuruldu
- [x] Host-side native test ortami kuruldu
- [x] UI ve radio queue testleri eklendi
- [ ] Gercek display adapter'i eklendi
- [ ] Gercek input adapter'i eklendi
- [ ] Gercek radio adapter'i eklendi
- [ ] Donanim smoke testleri tamamlandi

## API / Arayuzler
Core ile donanim arasindaki ana sinirlar:

- `Display`
- `Input`
- `RadioTransport`
- `Storage`
- `Clock`
- `PowerManager`

Bu arayuzler gelecekteki donanim entegrasyonunun resmi baglanti noktalaridir.

## Kurulum ve Calistirma
Gereksinim:
- PlatformIO Core

Derleme:

```powershell
platformio run -e esp32dev
```

Host-side test:

```powershell
platformio test -e native
```

## Handoff Notlari
### Where to start
Ilk bakilacak yerler:
- `lib/espekran/include/espekran/app/app_controller.h`
- `lib/espekran/src/app_controller.cpp`
- `src/main.cpp`

### Known issues
- README'de anlatilan hedef urun ile mevcut firmware arasinda hala buyuk kapsam farki var.
- Donanimla ilgili hicbir davranis fiziksel kart uzerinde dogrulanmis degil.
- Native testleri calistirmak icin host C/C++ derleyicisi gerekli; bu ortamda `gcc/g++` bulunmuyor.

### Decisions made
- Core kod Arduino ve FreeRTOS'tan ayrildi.
- Mock davranislar korundu ama acikca simulasyon olarak etiketlendi.
- Ilk test kapsami timing degil, state dogrulugu odakli secildi.

### Definition of done for the next milestone
- En az bir gercek donanim adapter'i arayuzlerden birine baglanmis olmali.
- Build bozulmadan devam etmeli.
- README, dogrulama durumunu gercekci sekilde yansitmaya devam etmeli.

## Donanim Smoke-Test Checklist
Fiziksel kart geldiginde ve kablolama hazir oldugunda uygulanacak minimum kontrol listesi:

- Seri portta boot banner gorunuyor mu
- FreeRTOS task'lari watchdog reset olmadan basliyor mu
- Wake button pini kararlı okunuyor mu
- D-Pad ADC degerleri ayrisiyor mu
- TFT ekran bilinen bir frame gosterebiliyor mu
- Touch koordinatlari kararlı geliyor mu
- NRF24L01 iki kart arasinda temel paket gonderip alabiliyor mu
- SD card mount ve basit dosya okuma calisiyor mu
- Sleep/wake akisi state bozmadan calisiyor mu
