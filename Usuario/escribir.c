#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_LENGTH	16
static char escribir[BUFFER_LENGTH];

int main(void)
{
	printf("\nPrograma de usuario para escribir i2c eeprom\n\n");
	int fd, cant_escribir, ret;

	printf("Abriendo el dispositivo.\n");
	fd = open("/dev/myeeprom", O_RDWR);		// myeeprom i2c_gonza
	if (fd < 0){
		printf("Error abriendo el dispositivo.\n");
		return -1;
	}

	printf("Ingrese el text a escribir en la memoria:");
	scanf("%s", escribir);

	if (strlen(escribir) > BUFFER_LENGTH)
		 escribir[BUFFER_LENGTH-1] = 0;

	printf("Se escribiran %d bytes del texto [%s]\n", strlen(escribir), escribir);
	ret = write(fd, escribir, strlen(escribir)+1);
	if (ret < 0){
		printf("Error escribiendo el dispositivo.\n");
		close(fd);
		return -1;
	}

	printf("Se escribieron [%d] bytes\n", ret);
	close(fd);
	return 0;
}
