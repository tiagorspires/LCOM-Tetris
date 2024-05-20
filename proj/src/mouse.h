#include <lcom/lcf.h>
#include "i8042.h"


int (mouse_subscribe_int)(uint8_t* bit_no);
int (mouse_unsubscribe_int)();
void (mouse_ih)();
int (send_command)(uint8_t command);
int (read_buffer)(uint8_t *data);
bool (is_input_buffer_full)();
int (change_data_report_mode)(uint8_t set);
void (mouse_event_handler)(struct packet *pp);


