#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_LENGTH	8
static char hora[BUFFER_LENGTH];

int main(void)
{
	printf("\nPrograma de usuario para leer la hora por i2c ds3231\n\n");
	int fd, ret;

	printf("Abriendo el dispositivo.\n");
	fd = open("/dev/myds3231", O_RDWR);
	if (fd < 0){
		printf("Error abriendo el dispositivo.\n");
		return -1;
	}

	ret = read(fd, hora, BUFFER_LENGTH);
	if (ret < 0){
		printf("Error leyendo el dispositivo.\n");
		close(fd);
		return -1;
	}

	printf("La hora actual es: %s\n", hora);
	close(fd);
	return 0;
}
