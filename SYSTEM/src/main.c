#include <asf.h>

#define SERIAL_CONNECTION	&USARTC0

int main (void)
{
	// Initialize all components
	board_init();
	sysclk_init();
	gfx_mono_init();
	
	// Set USART options
	static usart_rs232_options_t USART_SERIAL_OPTIONS = {
		.baudrate = 9600,
		.charlength = USART_CHSIZE_8BIT_gc,
		.paritytype = USART_PMODE_DISABLED_gc,
		.stopbits = false
	};

	// Initialize USART driver in RS232 mode
	usart_init_rs232(SERIAL_CONNECTION, &USART_SERIAL_OPTIONS);

	// Enable display backlight
	gpio_set_pin_high(NHD_C12832A1Z_BACKLIGHT);
	
	// Handle receiving characters and printing to screen
	while(true)
	{
		// Define character to be added and the string to be shown on the screen
		uint8_t char_to_add = '\0';
		char string[25] = ""; // Max of 24 characters (24 + null termination)
		
		// Keep count to make sure we only accept 24 characters
		uint8_t count = 0;
		
		// Accept the characters
		while(char_to_add != '\r' && count < 25)
		{
			// Grab the transmitted character from USART
			char_to_add = usart_getchar(SERIAL_CONNECTION);
			
			// Add the character to the string and increment count
			string[count] = char_to_add;
			count++;	
		}
		
		// Flash the backlight
		gpio_set_pin_low(NHD_C12832A1Z_BACKLIGHT);
		delay_ms(100);
		gpio_set_pin_high(NHD_C12832A1Z_BACKLIGHT);
		
		// Draw the string
		gfx_mono_draw_string(string, 10, 10, &sysfont);
	}
}
