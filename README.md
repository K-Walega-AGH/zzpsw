# ZZPSW - Zaawansowane Zagadnienia Programowania Systemów Wbudowanych

Projekty realizowane w ramach zaawansowanego kursu programowania systemów wbudowanych, skupiające się na wykorzystaniu systemu czasu rzeczywistego **FreeRTOS** do kontroli złożonych procesów sprzętowych.

## 🚀 Tematyka i technologie
Głównym celem jest przejście z modelu "bare-metal" na architekturę systemową opartą na wątkach (tasks), synchronizacji i komunikacji międzyprocesowej.

* **System operacyjny:** FreeRTOS (Real-Time Operating System).
* **Platforma:** LPC21xx / ARM7TDMI.
* **Kluczowe mechanizmy:**
    * Wielowątkowość i priorytetyzacja zadań.
    * Mechanizmy synchronizacji: **Semafory binarne i licznikowe**.
    * Komunikacja między wątkami: **Kolejki (Queues)**.
    * Zarządzanie czasem: System Delay (Tick-based).
    * Ochrona zasobów dzielonych i sekcje krytyczne.

## 📂 Zawartość projektów

### 1. Wielowątkowość i sterowanie serwomechanizmem
Implementacja sterownika serwomechanizmu opartego na automacie stanów, który został przeniesiony z przerwań timerowych do dedykowanego wątku FreeRTOS.
* Optymalizacja częstotliwości wywołań (100Hz / 200Hz).
* Analiza wywłaszczania (preemption) i jego wpływu na płynność ruchu.

### 2. Komunikacja przez kolejki (Queues)
Eliminacja zmiennych globalnych na rzecz bezpiecznych kolejek systemowych.
* Przekazywanie żądań pozycji (`uiDesiredPosition`) do modułu Servo.
* Zapewnienie atomowości operacji na danych 16/32-bitowych.

### 3. Synchronizacja i semafory
Wykorzystanie semaforów do ochrony dostępu do magistral komunikacyjnych (np. UART) oraz synchronizacji zdarzeń wyzwalanych przyciskami (debounce i obsługa w osobnym wątku).

### 4. Zaawansowana obsługa UART
Implementacja odbiornika i nadajnika UART w środowisku RTOS z wykorzystaniem buforów i powiadomień o zakończeniu transmisji.

## ⚙️ Narzędzia
* **Kompilator:** Keil uVision.
* **Konfiguracja:** Plik `FreeRTOSConfig.h` (dostosowanie częstotliwości ticka systemowego).
