/*
 * Thready.h
 *
 * Created: 21.04.2017 16:42.17
 *  Author: alari
 */

#ifndef THREADY_H_
#define THREADY_H_

#ifndef T_PASTE2
#define T_PASTE2_TOK(a, b) a##b
#define T_PASTE2(a, b) T_PASTE2_TOK(a, b)
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint-gcc.h>

#include "Uptime/Uptime.h" //depends on module Uptime!

#include "Config.h"
#include "Macros.h"
#include "Typedef.h"

#define tStart(context) threadyMacroStart(context)
#define tSleepMs(ms)                                                           \
  threadyMacroSleepMs(ms) // magab lihtsalt ja ootab järjekorda

#define tRepeat() threadyMacroRepeat() // nö for
#define tCount(count) threadyMacroCount(count)
#define tUntilOrCount(condition, count)                                        \
  threadyMacroUntilOrCount(condition, count)

#define tYield() threadyMacroYield()
#define tCancel(code) threadyMacroCancel(code) // see siis lõpetab thready ära

#define tSave()                                                                \
  threadyMacroSave() // tSave ei saa olla kuskil if() või for() sees. peab olema
                     // seal tStart(){} nö funktsiooni sees kohe.aga see teeb
                     // seda, et kui kutsud tSave(); . siis järgmine kord
                     // kutsudes tRestore() minna kse sinna kus save oli tehtud
#define tRestore() threadyMacroRestore()

#define tGetId() threadyMacroGetId()
#define tGetLoopIndex() threadyMacroGetLoopIndex()
#define tGetUserData() threadyMacroGetUserData()
#define tGetUserDataSize() threadyMacroGetUserDataSize()

/*
tSave();

if(!gpsOk()){
        tRestore(); //siin lahkub sellest funktsioonist ja järgmine kord
alustatakse tSave() juurest
}
*/

uint32_t threadyCreate(ThreadyContext *context, thready_t thread, void *userData); // userData can be NULL. Return

// thready ID - 0 on failure
bool threadyCancelId(uint32_t id);
bool threadyCancel(ThreadyContext *context);
void threadyLoop(void);
uint32_t threadyGetCount(void);

extern uint64_t threadyLoopCounter;

#endif /* THREADY_H_ */
