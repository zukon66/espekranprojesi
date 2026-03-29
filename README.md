# ESP32 Smart Communicator & Mini Console

Bu proje, ESP32 mikrodenetleyicisi üzerine inşa edilmiş, hem bir mesh mesajlaşma cihazı hem de retro/çok oyunculu bir mini el konsolu olarak tasarlanmış hibrit bir akıllı cihazdır. 

## 📌 Proje Özeti
Cihaz, dokunmatik ekranın hızı ile fiziksel butonların (D-Pad) ergonomisini birleştirir. NRF modülü üzerinden kendi kapalı ağı (Mesh Network) içinde mesajlaşma ve çok oyunculu oyun imkanı sunarken, Wi-Fi ve Bluetooth yetenekleriyle günlük bir asistan (hava durumu, saat, kablosuz hoparlör) görevi de görür.

## 🛠️ Donanım Bileşenleri
* *Mikrodenetleyici:* ESP32 (Tüm sistemin beyni, Wi-Fi ve BT Classic/BLE desteği)
* *Ekran:* 2.8 inç ILI9341 TFT LCD (240x320 Çözünürlük, SPI)
* *Dokunmatik Panel:* XPT2046 (Ekranla entegre, SPI)
* *Haberleşme Modülü:* NRF24L01 (Mesh ağı, eşler arası mesajlaşma ve oyun verisi, SPI)
* *Depolama:* MicroSD Kart Modülü (Medya, arayüz ikonları ve oyun verileri için, SPI)
* *Ses Sistemi:* Mini Amfi (Örn. PAM8403) ve 2 adet Laptop Hoparlörü (ESP32 I2S/DAC çıkışı ile)
* *Fiziksel Kontrolcüler:* 6 Adet Push Buton (Yukarı, Aşağı, Sağ, Sol, Seç/OK, Uyku/Uyanma)

## 🏗️ Donanım ve Pin Mimarisi Stratejisi
ESP32'nin kısıtlı pin sayısını optimize etmek için aşağıdaki yöntemler kullanılmıştır:
* *Ortak SPI Veriyolu:* Ekran, Dokunmatik Panel, SD Kart ve NRF modülü aynı SPI hattını (MOSI, MISO, SCK) paylaşır. Her modül kendi özel CS (Chip Select) pini ile yönetilir.
* *Analog D-Pad (Direnç Merdiveni):* 5 adet yön ve seçim tuşu, farklı değerlerdeki dirençlerle tek bir hatta bağlanarak ESP32'nin tek bir Analog (ADC) pininden okunur.
* *Güç Tasarrufu (Deep Sleep):* Uyku tuşu, cihazı uyandırmak için özel olarak RTC destekli bir pine (Örn. GPIO32/33) bağlanmıştır.

## 💻 Yazılım ve Arayüz (UI/UX) Özellikleri
Arayüz, LVGL veya TFT_eSPI kütüphaneleri kullanılarak modern bir "işletim sistemi" hissiyatıyla tasarlanmıştır. Cihaz ses kontrolünü fiziksel tuşlardan değil, doğrudan yazılım arayüzünden sağlar.

*Daima Açık Durum Çubuğu (Status Bar)*
* Saat, Wi-Fi, Bluetooth ve NRF bağlantı durumu, mini hava durumu ikonu.

*Ana Uygulamalar ve Menüler*
* *💬 Mesajlaşma:* Balon tasarımlı sohbet geçmişi ve dokunmatik ekran klavyesi ile NRF üzerinden metin veya Walkie-Talkie tarzı sesli mesaj gönderimi.
* *🎮 Oyunlar:* * Singleplayer: Retro klasik oyunlar (Flappy Bird, Snake vb.).
  * Multiplayer: NRF üzerinden gecikmesiz kablosuz PvP. (Özellikle satranç ve Amiral Battı gibi satır/sütun koordinat mantığıyla çalışan oyunlar).
* *📁 Dosya Yöneticisi:* SD kart üzerinden .txt okuma, .bmp/.jpg görüntüleme ve .wav formatında ses/müzik çalma.
* *🛠️ Araçlar:* Wi-Fi analizörü, OpenWeatherMap API ile çalışan tam ekran güncel hava durumu ve not defteri.
* *⚙️ Ayarlar:* Sessiz Mod aktivasyonu (haptic feedback/titreşim geçişi), Bluetooth hoparlör (A2DP Sink) modu, NRF kanal ayarları, ekran parlaklığı, yazılımsal ses kontrolü ve Tema değiştirici.

## 🚀 Gelecek Geliştirmeler / Yapılacaklar (To-Do)
- [ ] ESP32 pin haritasının (Pinout) kesinleştirilmesi.
- [ ] Breadboard üzerinde ekran ve NRF modülünün aynı SPI hattında test edilmesi.
- [ ] FreeRTOS kullanılarak arayüzün (Task 1) ve NRF dinleme işleminin (Task 2) aynı anda çalıştırılması.
- [ ] 3 Boyutlu yazıcı ile ergonomik bir kasa tasarımının yapılması.