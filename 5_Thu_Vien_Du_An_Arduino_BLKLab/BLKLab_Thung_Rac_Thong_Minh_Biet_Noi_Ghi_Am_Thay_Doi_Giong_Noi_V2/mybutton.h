#ifndef _MY_BUTTON_H_
#define _MY_BUTTON_H_

/* Cách sử dụng

 Khai báo chân + pin ID + biến kiểu dữ liệu Button  // uint8_t buttonPinOK = 3;
                                                    // #define BUTTON1_ID  1
                                                    //  Button buttonOK;
 Init trong hàm setup                               // button_init(&buttonOK,buttonPinOK,BUTTON1_ID);
 Trong Main                                         //  handle_button(&buttonOK);
*/


typedef void (*button_pressshort_callback)(int);
button_pressshort_callback pt_short = NULL;
typedef void (*button_presslong_callback)(int);
button_presslong_callback pt_long = NULL;
typedef enum {
  BUTTON_READ,
  BUTTON_WAIT_DEBOUND,
  BUTTON_WAIT_RELEASE_AND_CHECK_LONG_PRESS,
  BUTTON_WAIT_RELEASE,
}Button_State;

typedef struct {
  uint16_t gpio_pin;
  Button_State button_state;
  uint8_t current_status;
  uint8_t last_status;
  uint32_t t_long_press;
  uint8_t is_long_press;
  uint32_t time_debounce;
  uint8_t button_id;
}Button;

// Hàm xử lí có nhấn nhanh
void button_press_short_callback(uint8_t button_id);
// Hàm timeout nếu nhấn nút giữ lâu
void button_pressing_timeout_callback(uint8_t button_id);


// hàm xử lí nút nhấn, gọi hàm này trong main
void handle_button(Button *btn) {
  btn->current_status = digitalRead(btn->gpio_pin);
  switch(btn->button_state) {
    case BUTTON_READ:
      if((btn->current_status == 0 && btn->last_status == 1) ) {
            btn->time_debounce =  millis(); 
            btn->button_state = BUTTON_WAIT_DEBOUND;
      }
      break;
    case BUTTON_WAIT_DEBOUND:
      if(millis() - btn->time_debounce >= 20) {
        if(btn->current_status ==0 && btn->last_status ==1) {//nhan xuong 
          btn->t_long_press =millis();
          //last_status = current_status;
          btn->last_status = 0;
          btn->button_state = BUTTON_WAIT_RELEASE_AND_CHECK_LONG_PRESS;
        }
        else if(btn->current_status ==1 && btn->last_status ==0) {//nha ra
          btn->t_long_press =millis() - btn->t_long_press;
          if(btn->t_long_press <= 1000)
                pt_short(btn->button_id);
          btn->last_status = btn->current_status;
          btn->button_state = BUTTON_READ;
        }
        else {//khong dung
          btn->last_status = 1;
          btn->button_state = BUTTON_READ;
        }
      }
      break;
    case BUTTON_WAIT_RELEASE_AND_CHECK_LONG_PRESS:
      if(btn->current_status == 1 && btn->last_status == 0) {
        //truoc 3s dã nha phim
        btn->button_state = BUTTON_WAIT_DEBOUND;
        btn->time_debounce = millis();
      }
      else if(millis()- btn->t_long_press >= 1500) {
        pt_long(btn->button_id);
        btn->button_state = BUTTON_WAIT_RELEASE;
      }   
      break;
    case BUTTON_WAIT_RELEASE:
      if(btn->current_status == 1 && btn->last_status == 0) {
        btn->button_state = BUTTON_WAIT_DEBOUND;
        btn->time_debounce =millis();
      }
      break;
    default:   
      break;
  }
}


void button_init(Button *btn, uint16_t GPIO_Pin,uint8_t button_id) {
  btn->gpio_pin = GPIO_Pin;
  btn->current_status =1;
  btn->last_status =1;
  btn->button_id = button_id;
}


void button_pressshort_set_callback(void *cb) {
    pt_short = (button_pressshort_callback)cb;
}
void button_presslong_set_callback(void *cb) {
    pt_long = (button_presslong_callback)cb;
}


#endif