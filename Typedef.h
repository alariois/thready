/*
 * Typedef.h
 *
 * Created: 24.04.2017 11:00.45
 *  Author: alari
 */


#ifndef THREADY_TYPEDEF_H_
#define THREADY_TYPEDEF_H_

typedef enum{
	THREADY_STATUS_STARTING,
	THREADY_STATUS_RUNNING,
	THREADY_STATUS_DONE,
	THREADY_STATUS_CANCELLED
} ThreadyStatus;

typedef void (*thready_t)(void *context, void *userData);

typedef struct{
	uint32_t userDataSize;
	void* userData;
	thready_t thready;
	void (*callback)(void *);
} ThreadyOptions;

typedef struct{
	ThreadyOptions options;
	void* pos;
	void* posSleep;
	void* savedPos;
	uint32_t id;
	uint16_t repeatCount;
	uint64_t timestampDelay;
	ThreadyStatus status;
	uint8_t exitCode; //8bit exit code
	uint8_t posIndex;
	void *prev;
	void *next;
} ThreadyContext;

#endif /* THREADY_TYPEDEF_H_ */
