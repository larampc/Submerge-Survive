#include <lcom/lcf.h>
#include "rtc.h"

int rtc_hook_id;
static bool BCD_ENCODING;

uint8_t hours = 0, minutes = 0, seconds = 0;
int rtc_counter = 0, elapsed_time = 0;

inline int bcd_to_bin(uint8_t* bcd){
	if(bcd == NULL){
		printf("in %s: null pointer\n", __func__);
		return 1;
	}
	*bcd = ((*bcd & 0xF0) >> 4) * 10 + (*bcd & 0x0F);
	return 0;
}
inline int bin_to_bcd(uint8_t* dec){
	if(dec == NULL){
		printf("in %s: null pointer\n", __func__);
		return 1;
	}
	uint8_t ones = 0, tens = 0, temp = 0;

    ones = *dec%10; 
    temp = *dec/10; 
    tens = temp<<4;  
    *dec = tens + ones;
	return 0;
}


int rtc_read_reg(int port, uint8_t* value){
	if(sys_outb(RTC_ADDR_REG, port) != 0) return 1;
	if(util_sys_inb(RTC_DATA_REG, value) != 0) return 1;
	return 0;
}
int rtc_write_reg(int port, uint8_t value){
	if(sys_outb(RTC_ADDR_REG, port) != 0) return 1;
	if(sys_outb(RTC_DATA_REG, value) != 0) return 1;
	return 0;
}


int rtc_subscribe_update_interrupts() {
	uint8_t regB;
	if (rtc_read_reg(RTC_STATUS_B_REG, &regB)) {
		printf("in %s: error reading statusB\n", __func__);
		return 1;
	}
	if (rtc_write_reg(RTC_STATUS_B_REG, (regB | RTC_STB_UIE ))) {
		printf("in %s: error configuring statusB\n", __func__);
		return 1;
	}
	return 0;
}

int rtc_unsubscribe_update_interrupts() {
	uint8_t regB;
	if (rtc_read_reg(RTC_STATUS_B_REG, &regB)) {
		printf("in %s: error reading statusB\n", __func__);
		return 1;
	}
	regB &= ~(RTC_STB_UIE);
	if (rtc_write_reg(RTC_STATUS_B_REG, regB)) {
		printf("in %s: error configuring statusB\n", __func__);
		return 1;
	}
	return 0;
}

int rtc_subscribe_alarm_updates() {
	uint8_t regB;
	if (rtc_read_reg(RTC_STATUS_B_REG, &regB)) {
		printf("in %s: error reading statusB\n", __func__);
		return 1;
	}
	if (rtc_write_reg(RTC_STATUS_B_REG, (regB | RTC_STB_AIE ))) {
		printf("in %s: error configuring statusB\n", __func__);
		return 1;
	}
	return 0;
}

int rtc_unsubscribe_periodic_interrupts() {
	uint8_t regB;
	if (rtc_read_reg(RTC_STATUS_B_REG, &regB)) {
		printf("in %s: error reading statusB\n", __func__);
		return 1;
	}
	regB &= ~(RTC_STB_PIE);
	if (rtc_write_reg(RTC_STATUS_B_REG, regB)) {
		printf("in %s: error configuring statusB\n", __func__);
		return 1;
	}
	return 0;
}

int rtc_subscribe_periodic_interrupts(uint8_t frequency) {
	uint8_t regA, regB;
	if (rtc_read_reg(RTC_STATUS_A_REG, &regA)) {
		printf("in %s: error reading statusA\n", __func__);
		return 1;
	}
	if (rtc_write_reg(RTC_STATUS_A_REG, ((regA &  0xF0) | frequency))) {
		printf("in %s: error configuring statusA\n", __func__);
		return 1;
	}
	if (rtc_read_reg(RTC_STATUS_B_REG, &regB)) {
		printf("in %s: error reading statusB\n", __func__);
		return 1;
	}
	if (rtc_write_reg(RTC_STATUS_B_REG, (regB | RTC_STB_PIE ))) {
		printf("in %s: error configuring statusB\n", __func__);
		return 1;
	}
	rtc_counter = 0;
	return 0;
}

int rtc_init(){
	uint8_t regB;
	if (rtc_read_reg(RTC_STATUS_B_REG, &regB)) {
		printf("in %s: error reading statusB\n", __func__);
		return 1;
	}
	regB &= ~(RTC_STB_AIE | RTC_STB_PIE | RTC_STB_UIE);
	if (rtc_write_reg(RTC_STATUS_B_REG, regB)) {
		printf("in %s: error configuring statusB\n", __func__);
		return 1;
	}
	BCD_ENCODING = ((regB & RTC_STB_BIN) == 0);
	char date[50];
	if(rtc_subscribe_alarm_updates()) {
		printf("in %s: error enabling update interrupts\n", __func__);
		return 1;
	}
	rtc_set_alarm(RTC_DONT_CARE, RTC_DONT_CARE, RTC_DONT_CARE);
	if(rtc_get_time(date)) {
		printf("in %s: error retrieving current date\n", __func__);
		return 1;
	};
    sscanf(date, "%02hhu:%02hhu %hhu", &hours, &minutes, &seconds); 
	return 0;
}

int rtc_subscribe_int(uint8_t* bit_no) {
	if (bit_no == NULL){
        printf("in %s: null pointer\n", __func__);
		return 1;
	}
	uint8_t regC;
	if (rtc_read_reg(RTC_STATUS_C_REG, &regC)) {
		printf("in %s: error reading statusB\n", __func__);
		return 1;
	}
	rtc_hook_id = *bit_no;
	*bit_no = BIT(*bit_no);
	if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &rtc_hook_id) != 0) {
        printf("in %s: failed to set rtc policy\n", __func__);
        return 1;
    }
	return 0;
}

int rtc_unsubscribe_int(){
	if (sys_irqrmpolicy(&rtc_hook_id) != 0){
        printf("in %s: failed to remove rtc policy\n", __func__);
		return 1;
  }
  return 0;
}


void wait_valid_rtc() {
	for (uint8_t regA = 0; ; ) {
		rtc_read_reg(RTC_UIP_REG, &regA);
		if(!(regA & RTC_STA_UIP)) break;
	}
}

int rtc_get_date(char* str) {
	if(str == NULL){
		printf("in %s: null pointer\n", __func__);
		return 1;
	}
	wait_valid_rtc();
	uint8_t mday, month, year;
	rtc_read_reg(RTC_MDAY_REG, &mday);
	rtc_read_reg(RTC_MONTH_REG, &month);
	rtc_read_reg(RTC_YEAR_REG, &year);
	if( BCD_ENCODING ){
		if(bcd_to_bin(&mday) || 
		bcd_to_bin(&month) || 
		bcd_to_bin(&year))
			return 1;
	}
	sprintf(str, "%02hhu/%02hhu/%hhu\n", mday, month, year);
	return 0;
}

int rtc_get_time(char* str) {
	if(str == NULL){
		printf("in %s: null pointer\n", __func__);
		return 1;
	}
	wait_valid_rtc();
	uint8_t seconds, minutes, hours;
	rtc_read_reg(RTC_SECONDS_REG, &seconds);
	rtc_read_reg(RTC_MINUTES_REG, &minutes);
	rtc_read_reg(RTC_HOURS_REG, &hours);
	if( BCD_ENCODING ){
		if(bcd_to_bin(&hours) || 
		bcd_to_bin(&minutes) || 
		bcd_to_bin(&seconds))
			return 1;
	}
	sprintf(str, "%02hhu:%02hhu %hhu\n", hours, minutes, seconds);
	return 0;
}

int rtc_unsubscribe_alarm_updates() {
	uint8_t regB;
	if (rtc_read_reg(RTC_STATUS_B_REG, &regB)) {
		printf("in %s: error reading statusB\n", __func__);
		return 1;
	}
	regB &= ~(RTC_STB_AIE);
	if (rtc_write_reg(RTC_STATUS_B_REG, regB)) {
		printf("in %s: error configuring statusB\n", __func__);
		return 1;
	}
	return 0;
}

int rtc_set_alarm(uint8_t hour, uint8_t minute, uint8_t second) {
	if(BCD_ENCODING){
		if(hour != RTC_DONT_CARE && bin_to_bcd(&hour))
			return 1;
		if(minute != RTC_DONT_CARE && bin_to_bcd(&minute))
			return 1; 
		if(second != RTC_DONT_CARE && bin_to_bcd(&second))
			return 1;
	}
	if(rtc_write_reg(RTC_HOURS_ALARM_REG, hour) || 
		rtc_write_reg(RTC_MINUTES_ALARM_REG, minute) ||
		rtc_write_reg(RTC_SECONDS_ALARM_REG, second)){
			printf("in %s: error setting alarm\n", __func__);
			return 1;
		}
	rtc_subscribe_alarm_updates();
	return 0;
}

void update_date_time(){
	seconds++;
	if(seconds == 60){
		minutes++;
		seconds = 0;
		if(minutes == 60){
			hours++;
			minutes = 0;
			if(hours == 24) hours = 0;
		}
	}
}

bool rtc_ih(){
	uint8_t data;
	rtc_read_reg(RTC_STATUS_C_REG, &data);
	if(data & RTC_STC_AF) {
		update_date_time();
		return false;
		// rtc_unsubscribe_alarm_updates();
	}
	if (data & RTC_STC_UF){
		return false;
		//update_date_time();
	}
	if (data & RTC_STC_PF){
		rtc_counter++;
		rtc_counter %= 249;
		elapsed_time++;
		return true;
		// update_date_time();
	}
	return false;
}
