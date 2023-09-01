# ESP32_ESPNOW_RSSI
Esto funciona sobre dos ESP32 y se basa en parte en los ejemplos del curso de ESP32 de SARA & RUI SANTOS

La idea es leer entradas digitales y temperatura desde el "emisor" y pasarlas al "receptor" que tiene unos LED y un display OLED usando el protocolo ESP_NOW de Espressif

Hay un LED en particular que indica si el receptor "escucha" al emisor (cuando esta encendido) y agregué una última linea al OLED con el dato del RSSI del paquete recibido.

Lo he probado en terreno y en efecto al alejarme del "emisor" el valor de RSSI se incrementa (tal cual deberia ser) y al acercarme, decrece.

Este código contiene partes obtenidas del foro de ESP32 de Espressif copiadas y pegadas, basadas en la idea de https://www.esp32.com/viewtopic.php?t=13889 y modificadas parcialmente.

Espero que mi códgio sea entendible...

Si esta información te resulta útil e interesante, invitame un cafecito!!!
https://cafecito.app/marce_ferra

Desde fuera de Argentina en:
https://www.buymeacoffee.com/marceferra

If you found this information useful and interesting, buy me a cafecito!!!
https://www.buymeacoffee.com/marceferra
