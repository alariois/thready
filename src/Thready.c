/*
 * Thready.c
 *
 * Created: 21.04.2017 16:42.24
 *  Author: alari
 */

#include <stddef.h>
#include <string.h>

#include "../Thready.h"
// TODO:  remove debugf dependency
// #include "Debugf/Debugf.h"
// #include "Debugf/PreProc.h"

static ThreadyContext *_threadyFirstElement;
static ThreadyContext *_threadyLastElement;
static uint32_t _id = 1;
static uint32_t _threadCount = 0;

static bool _threadyCancelUnsafe(ThreadyContext *context);

uint32_t threadyCreate(ThreadyContext *context, thready_t thready,
                       void *userData) {
  if (context->status == THREADY_STATUS_RUNNING) {
    // dbgI(State, "Attempt to create existing thread blocked!");
    return 0;
  }

  if (context == NULL)
    return 0;
  if (thready == NULL)
    return 0;

  if (_threadyLastElement != NULL) {
    _threadyLastElement->next = context;
  }

  if (_threadyFirstElement == NULL)
    _threadyFirstElement = context;
  ThreadyOptions options = context->options;
  memset(context, 0, sizeof(ThreadyContext));
  context->options = options;
  context->id = _id++;
  context->options.thready = thready;
  context->options.userData = userData;

  context->prev = _threadyLastElement;
  _threadyLastElement = context;

  _threadCount++;

  // dbgV(State, "CREATE thread id=%2lu count=%2lu self = %10p prev = %10p next
  // = %10p first = %10p last = %10p", context->id, _threadCount, context,
  // context->prev, context->next, _threadyFirstElement, _threadyLastElement);

  return context->id;
}

/*
static void _tloop(void *context){
        tStart(){
                static uint8_t i = 0;

                asm volatile("NOP");
                dbgV(DBG_MISC, "t start!");
                asm volatile("NOP");

                tYield();
                asm volatile("NOP");
                dbgV(DBG_MISC, "t yeald0!");
                asm volatile("NOP");

                tRepeat(){
                        dbgV(DBG_MISC, "t LOOP!");
                } tCount(3);

                tSave();

                tRepeat(){
                        i++;
                        dbgV(DBG_MISC, "t repeat until i == 11! i = %d", i);
                } tUntilOrCount(i == 11, 4){
                        dbgV(DBG_MISC, "COUNT REACHED i = %d", i);
                        //COUNT reached
                        tRestore();
                }

                dbgV(DBG_MISC, "out of repeat");

                i=0;
                dbgV(DBG_MISC, "start delayed output");
                tSleepMs(500);

                i++;
                dbgV(DBG_MISC, "delay 1 = %d", i);
                tSleepMs(500);

                tSave();

                i++;
                dbgV(DBG_MISC, "delay 2 = %d", i);
                tSleepMs(500);

                i++;
                dbgV(DBG_MISC, "delay 3 = %d", i);
                tSleepMs(500);

                i++;
                dbgV(DBG_MISC, "delay 4 = %d", i);

                if(i < 10){
                        dbgV(DBG_MISC, "RESTORE SLEEP!");
                        tRestore();
                }

                i = 0;
                tRepeat(){
                        dbgV(DBG_MISC, "delayed repeat %d", i++);
                        tSleepMs(1000);
                } tCount(5);

                i = 0;
                dbgV(DBG_MISC, "DOUBLE DELAY START");
                tRepeat(){
                        dbgV(DBG_MISC, "double delayed repeat ONE %d", i);
                        tSleepMs(500);
                        dbgV(DBG_MISC, "double delayed repeat TWO %d", i++);
                        tSleepMs(500);
                } tCount(5);

                i = 0;
                dbgV(DBG_MISC, "TRIPLE DELAY START");
                tRepeat(){
                        dbgV(DBG_MISC, "triple delayed repeat ONE %d", i);
                        tSleepMs(500);
                        dbgV(DBG_MISC, "triple delayed repeat TWO %d", i);
                        tSleepMs(500);
                        dbgV(DBG_MISC, "triple delayed repeat THREE %d", i++);
                        tSleepMs(500);
                } tCount(5);
        }
}*/

static bool _threadyCancelUnsafe(ThreadyContext *context) {
  ThreadyContext *prev = context->prev;
  ThreadyContext *next = context->next;

  if (prev != NULL) {
    prev->next = next;
  } else {
    _threadyFirstElement = next;
  }

  if (next != NULL) {
    next->prev = prev;
  } else {
    _threadyLastElement = prev;
  }

  _threadCount--;

  if (context->options.callback != NULL) {
    context->options.callback(context);
  }

  // dbgV(State, "REMOVE thread id=%2lu count=%2lu self = %10p prev = %10p next
  // = %10p first = %10p last = %10p", context->id, _threadCount, context,
  // context->prev, context->next, _threadyFirstElement, _threadyLastElement);

  return true;
}

bool threadyCancel(ThreadyContext *context) {
  ThreadyContext *c = _threadyFirstElement;
  while (c != NULL) {
    if (context == c) {
      return _threadyCancelUnsafe(c);
    }
    c = c->next;
  }
  return false;
}

void threadyLoop(void) {
  ThreadyContext *context = _threadyFirstElement;

  while (context != NULL) {
    context->options.thready(context, context->options.userData);
    context = context->next;
  }

  context = _threadyFirstElement;
  while (context != NULL) {
    if (context->status != THREADY_STATUS_RUNNING) {
      _threadyCancelUnsafe(context);
    }
    context = context->next;
  }

  /*
  static ThreadyStatus status = THREADY_STATUS_STARTING;
  static ThreadyContext context;

  _tloop(&context);

  if(context.status != status){
          status = context.status;
          dbgD(DBG_MISC, "STATUS CHANGED: %d", status);
  } */
}
