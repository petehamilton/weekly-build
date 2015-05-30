/*
*  kernel.c
*/
void kmain(void)
{
    const char *str = "my first linux kernel via OSX using vagrant + VNC";
    char *vidptr = (char*)0xb8000;  //video memory begins here.
    unsigned int i = 0;

    /*
    * this loops clears the screen
    * there are 25 lines each of 80 columns; each element takes 2 bytes (16bit)
    */
    while(i < 80 * 25 * 2) {
        vidptr[i] = ' '; //Set character to be a space
        vidptr[i+1] = 0x07; // attribute-byte - in this case, light grey on black screen

        j = i + 2;
    }

    i = 0; /* reset i so we can re-use it */
    unsigned int j = 0;

    // this loop writes the string to video memory until it's hits the null string terminator
    while(str[i] != '\0') {
        vidptr[j] = str[i]; // use the string char's ascii value
        vidptr[j+1] = 0x07; // attribute-byte: give character black bg and light grey fg

        i++;
        j = j + 2;
    }
    return;
}
