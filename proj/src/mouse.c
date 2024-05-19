#include <lcom/lcf.h>
#include "i8042.h"
#include "mouse.h"


int kbc_hook_id=2;
struct packet pp;
int i=0;
bool valid_packet=true;
bool update_count=false;

int (mouse_subscribe_int)(uint8_t* bit_no){
  if(bit_no == NULL) return 1;
  *bit_no = kbc_hook_id;
  if(sys_irqsetpolicy(IRQ_LINE_MOUSE, IRQ_REENABLE|IRQ_EXCLUSIVE, &kbc_hook_id)) return 1;
  return 0;
}

int (mouse_unsubscribe_int)(){
  if(sys_irqrmpolicy(&kbc_hook_id)) return 1;
  return 0;
}

void (mouse_ih)(){
  uint8_t status;

  if(util_sys_inb(STATUS_PORT,&status)) return ;

  if((status & MOUSE) && (status & OUT_BUF_FULL)){

    uint8_t byte;

    if(util_sys_inb(DATA_PORT,&byte)) return ;

    if(!(status & ERROR)){

        if(i==0 && (!(byte & FIRST_BYTE))){
          i--;
        }

        else{
          pp.bytes[i]=byte;
        }
        if(i==2){ 

          if(!valid_packet){
            valid_packet=true;
          }else{

            pp.lb = pp.bytes[0] & LEFT_BUTTON;
            pp.mb = pp.bytes[0] & MIDDLE_BUTTON;
            pp.rb = pp.bytes[0] & RIGHT_BUTTON;
            pp.x_ov = pp.bytes[0] & X_OVFL;
            pp.y_ov = pp.bytes[0] & Y_OVFL;
            bool x_neg = pp.bytes[0] & X_SIGN;
            bool y_neg = pp.bytes[0] & Y_SIGN;

            int16_t delta_x=pp.bytes[1];
            int16_t delta_y=pp.bytes[2];
            
            if(x_neg){
              delta_x |= SIGN_EXTENSION;
            }
            if(y_neg){
              delta_y |= SIGN_EXTENSION;
            }

            pp.delta_x = delta_x;
            pp.delta_y = delta_y;

            mouse_print_packet(&pp);
            update_count=true;
            i=-1;
          }
        }
    }
    else valid_packet=false;

    i++;
  }
}




