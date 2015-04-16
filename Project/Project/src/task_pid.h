/*
 * task_pid.h
 *
 * Created: 2015-03-18 16:27:57
 *  Author: Andreas & Peter
 */ 


#ifndef TASK_PID_H_
#define TASK_PID_H_

#define TASK_PID_STACK_SIZE (2048/sizeof(portSTACK_TYPE))
#define TASK_PID_STACK_PRIORITY (tskIDLE_PRIORITY)

#define M 3

void task_pid(void *pvParameters);

void set_setPoint(uint16_t);

void set_propotional_gain(uint8_t);

void set_time_integral(uint8_t);

void set_time_derivate(uint8_t);

void set_sample_time(uint8_t);

void reset(void);

void regulator(void);

void matlab_configure(void);

#endif /* TASK_PID_H_ */