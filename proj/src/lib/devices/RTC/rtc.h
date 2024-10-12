#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#define RTC_IRQ 8

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define RTC_STATUS_A_REG 0xA
#define RTC_STATUS_B_REG 0xB
#define RTC_STATUS_C_REG 0xC

#define RTC_SECONDS_REG 0x0
#define RTC_SECONDS_ALARM_REG 0x1
#define RTC_MINUTES_REG 0x2
#define RTC_MINUTES_ALARM_REG 0x3
#define RTC_HOURS_REG 0x4
#define RTC_HOURS_ALARM_REG 0x5
#define RTC_WDAY_REG 0x6
#define RTC_MDAY_REG 0x7
#define RTC_MONTH_REG 0x8
#define RTC_YEAR_REG 0x9

#define RTC_UIP_REG 0xA
#define UIP_MICROS 244

#define RTC_INT_REG 0xC
#define RTC_UPDATE_REG 0xB
#define RTC_INT_BIT BIT(5)

#define RTC_STA_RS(n) (BIT(n))
#define RTC_STA_UIP (BIT(7))

#define RTC_STB_SET (BIT(7))
#define RTC_STB_PIE (BIT(6))
#define RTC_STB_AIE (BIT(5))
#define RTC_STB_UIE (BIT(4))
#define RTC_STB_SQWE (BIT(3))
#define RTC_STB_BIN (BIT(2))
#define RTC_STB_24_12 (BIT(1))

#define RTC_STC_IRQF (BIT(7))
#define RTC_STC_PF (BIT(6))
#define RTC_STC_AF (BIT(5))
#define RTC_STC_UF (BIT(4))

#define RTC_EVENT_REG 0xC
#define RTC_DONT_CARE (BIT(7) | BIT(6))

#define PERIODIC_16 BIT(2) | BIT(3)
#define PERIODIC_1024 BIT(1) | BIT(2)

/** @file */
/** @defgroup sp Sp
 *
 *
 * @brief Real-Time Clock (RTC) management module
 * @{
 */


/**
 * @brief Initializes the RTC, disabling all interrupts and setting BCD encoding if necessary.
 * 
 * @return Zero on success, non-zero otherwise.
 */
int rtc_init();
/**
 * @brief Sets an alarm at the specified time.
 * 
 * @param hour Alarm hour.
 * @param minute Alarm minute.
 * @param second Alarm second.
 * @return Zero on success, non-zero otherwise.
 */
int rtc_set_alarm(uint8_t hour, uint8_t minute, uint8_t second);
/**
 * @brief Retrieves the current date as a formatted string.
 * 
 * @param str Pointer to the string to store the date.
 * @return Zero on success, non-zero otherwise.
 */
int rtc_get_date(char* str);
/**
 * @brief Retrieves the current time as a formatted string.
 * 
 * @param str Pointer to the string to store the time.
 * @return Zero on success, non-zero otherwise.
 */
int rtc_get_time(char* str);
/**
 * @brief Subscribes to RTC interrupts.
 * 
 * @param bit_no Pointer to the bit number to be used for interrupt notifications.
 * @return Zero on success, non-zero otherwise.
 */
int rtc_subscribe_int(uint8_t* bit_no);
/**
 * @brief Unsubscribes from RTC interrupts.
 * 
 * @return Zero on success, non-zero otherwise.
 */
int rtc_unsubscribe_int();
/**
 * @brief RTC interrupt handler to manage received interrupts.
 * 
 * @return bool True if periodic interrupt, false otherwise.
 */
bool rtc_ih();
/**
 * @brief Subscribes to periodic interrupts with the specified frequency.
 * 
 * @param frequency Frequency of the periodic interrupts.
 * @return Zero on success, non-zero otherwise.
 */
int rtc_subscribe_periodic_interrupts(uint8_t frequency);

/**
 * @brief Unsubscribes from periodic interrupts.
 * 
 * @return Zero on success, non-zero otherwise.
 */
int rtc_unsubscribe_periodic_interrupts();

#endif


