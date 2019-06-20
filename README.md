# MSE_19imd
Implementación de manejadores de dispositivos

### Entregables:

- Código fuente del device driver que hayan desarrollado.
<br />https://github.com/companyroberto/MSE_19imd/blob/master/Drivers/myeeprom/myeeprom.c
<br />https://github.com/companyroberto/MSE_19imd/blob/master/Drivers/myds3231/myds3231.c
<br />https://github.com/companyroberto/MSE_19imd/blob/master/Drivers/myds3231_kernel/myds3231.c

- Código fuente de la aplicación de usuario.
<br />https://github.com/companyroberto/MSE_19imd/blob/master/Usuario/leer.c
<br />https://github.com/companyroberto/MSE_19imd/blob/master/Usuario/escribir.c
<br />https://github.com/companyroberto/MSE_19imd/blob/master/Usuario/hora.c
<br />https://github.com/companyroberto/MSE_19imd/blob/master/Usuario/rtcioctl.c
<br />https://github.com/companyroberto/MSE_19imd/blob/master/Usuario/rtcioctl_set.c
<br />https://github.com/companyroberto/MSE_19imd/blob/master/Usuario/rtcpie.c

- Device tree "custom".
<br />https://github.com/companyroberto/MSE_19imd/blob/master/DeviceTree/sun8i-h2-plus-orangepi-zero-tito.dts

- Los Makefiles para compilar los módulos se encuentran en la misma carpeta y en la aplicación de usuario no es necesario.

- Presentación del Trabajo Práctico.
<br />https://github.com/companyroberto/MSE_19imd/blob/master/Documentacion/Presentaci%C3%B3n%20Trabajo%20Final.pdf


### Demostración:

Para utilizar el driver, ejecutar los siguientes pasos desde el linux de la Orange Pi ingresando como root:

1) Driver y programa de usuario presentado en clase para validar el funcionamiento.
<br />insmod ./myeeprom/myeeprom.ko
<br />./usuario/leer
<br />./usuario/escribir
<br />rmmod myeeprom

2) Drivers nativos para familiarizarse con el funcionamiento del dispositivo.
<br />i2cdetect -r 0
<br />i2cget -y 0 0x68 0 b
<br />i2cdump -y -r 0-2 0 0x68 b

3) Driver propio y programa de usuario para leer la hora del RTC.
<br />insmod ./myds3231/myds3231.ko
<br />./usuario/hora
<br />rmmod myds3231

4) Driver integrado al kernel y programa de usuario que utilizando "ioctl" lee y escribe la hora del RTC.
<br />insmod ./myds3231_kernel/myds3231.ko
<br />./usuario/rtcioctl
<br />./usuario/rtcioctl_set
<br />./usuario/rtcioctl
<br />rmmod myds3231


### Notas:
- ./linux-kernel-labs/src/linux/Documentation/devicetree/bindings/rtc/rtc-ds1307.txt	(configurar dts compatible en kernel)
- ./linux-kernel-labs/src/linux/drivers/rtc/rtc-ds1307.c				(driver compatible en el kernel)
- ./linux-kernel-labs/src/linux/Documentation/rtc.txt					(explicación para el usuario)
- http://man7.org/linux/man-pages/man4/rtc.4.html					(utilización de RTC en linux)
