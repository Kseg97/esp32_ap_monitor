# ESP32 AP WEB MONITOR
Interfaz HMI para monitor de presión y flujo. Contiene script para el ESP32 (monitor web server), script para formatear la EEPROM (SPIFFS: SPI File System, usado para archivo grandes como la página que es servida) y el plugin SPIFFS.

## Instalación 🚀

0. Instalar ESP32 e instalar el plugin SPIFFS (.jar)
	b. Para instalar el plugin, solo copiar ESP32FS dentro de la carpeta de instalación de arduino/tools/
	
	![Image description](https://github.com/Kseg97/esp32_ap_monitor/blob/master/spiffs.png)
1. Descargar este código junto a la carpeta data

2. Formatear SPIFFS 
	b. Recomiendo cerrar el monitor serial
3. Subir código
4. Esp sketch data spiffs upload
	b. Ya se puede usar el monitor
5. Listo, reiniciar ESP y conectar el celular o computador al AP

Cada modificación al index.html requiere repetir los pasos desde el 2 (para vaciar la EEPROM via SPIFFS)

![Image description](https://github.com/Kseg97/esp32_ap_monitor/blob/master/description.png)
