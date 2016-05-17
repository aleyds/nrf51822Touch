#ifndef __LIB_RTC_H__
#define __LIB_RTC_H__
#define LFCLK_FREQUENCY           (32768UL)                               /**< LFCLK frequency in Hertz, constant. */
#define RTC_FREQUENCY             (8UL)                                   /**< Required RTC working clock RTC_FREQUENCY Hertz. Changable. */
#define COMPARE_COUNTERTIME       (3UL)                                   /**< Get Compare event COMPARE_TIME seconds after the counter starts from 0. */
#define COUNTER_PRESCALER         ((LFCLK_FREQUENCY / RTC_FREQUENCY) - 1)   /* f = LFCLK/(prescaler + 1) */

#define RTC0_IRQ_PRI            APP_IRQ_PRIORITY_LOW

uint32_t lib_rtc_init(void);


#endif

