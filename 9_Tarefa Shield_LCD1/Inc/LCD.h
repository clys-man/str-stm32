
#ifndef LCD_H_
#define LCD_H_


void GPIO_init(void);
void send_to_lcd(int data, int rs);
void lcd_send_cmd(char cmd);
void lcd_init(void);
void lcd_send_data (char data);
void lcd_clear (void);
void lcd_put_cur(int row, int col);
void lcd_send_string (char *str);

#endif
