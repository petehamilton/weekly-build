/*
* kernel.c
*/

void kmain(void)
{
	const char *str = "my first kernel via OSX using vagrant + VNC";
	char *vidptr = (char*)0xb8000; // video memory begins here
	unsigned int i = 0;
	unsigned int j = 0;
	
	/* clear the screen w/ this loop */
	/* 25 lines, 80 cols, each elem = 2bytes */
	while(j < 80 * 25 * 2) {
		/* char byte */
		vidptr[j] = ' ';

		/* attribute byte */
		vidptr[j+1] = 0x07; /* black bg, light grey fg */
		j = j + 2;
	}
	j = 0;

	/* Write string to video memory */
	while(str[j] != '\0') {
		vidptr[i] = str[j];
		vidptr[i+1] = 0x07; /* Black bg, light grey fg */
		++j;
		i = i + 2;
	}

	return;
}
