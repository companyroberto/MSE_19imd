#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_LENGTH	16
static char leer[BUFFER_LENGTH];

int main(void)
{
	printf("\nPrograma de usuario para leer i2c eeprom\n\n");
	int fd, cant_lectura, ret;

	printf("Abriendo el dispositivo.\n");
	fd = open("/dev/myeeprom", O_RDWR);		// myeeprom i2c_gonza
	if (fd < 0){
		printf("Error abriendo el dispositivo.\n");
		return -1;
	}

	printf("Bytes a leer desde el dispositivo:");
	scanf("%d", &cant_lectura);

	if (cant_lectura > BUFFER_LENGTH)
		cant_lectura = BUFFER_LENGTH;

	printf("Se leeran %d bytes\n", cant_lectura);
	printf("Leyendo desde el dispositivo...\n");
	ret = read(fd, leer, cant_lectura);
	if (ret < 0){
		printf("Error leyendo el dispositivo.\n");
		close(fd);
		return -1;
	}

	printf("Mensaje leído: [%s]\n", leer);
	close(fd);
	return 0;
}
