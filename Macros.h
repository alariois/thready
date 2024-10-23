/*
 * Macros.h
 *
 * Created: 24.04.2017 12:03.45
 *  Author: alari
 */

#ifndef THREADY_MACROS_H_
#define THREADY_MACROS_H_

#define THREADY_POSITION_NAME T_PASTE2(localThreadyVarPos, __LINE__)

#define threadyMacroStart(context)                                             \
  ThreadyContext *localThreadyVarContext = (ThreadyContext *)(context);        \
  if (localThreadyVarContext->status == THREADY_STATUS_DONE ||                 \
      localThreadyVarContext->status == THREADY_STATUS_CANCELLED)              \
    return;                                                                    \
  if (localThreadyVarContext->pos != NULL)                                     \
    goto * localThreadyVarContext->pos;                                        \
  localThreadyVarContext->pos = &&THREADY_POSITION_NAME;                       \
  localThreadyVarContext->status = THREADY_STATUS_RUNNING;                     \
  THREADY_POSITION_NAME:                                                       \
  for (; localThreadyVarContext->status == THREADY_STATUS_RUNNING;             \
       localThreadyVarContext->status = THREADY_STATUS_DONE)

#define threadyMacroSleepMs(ms)                                                \
  localThreadyVarContext->timestampDelay =                                     \
      uptimeGetMs() + ((ms) >= 0 ? (ms) : 0);                                  \
  localThreadyVarContext->posSleep = localThreadyVarContext->pos;              \
  localThreadyVarContext->pos = &&THREADY_POSITION_NAME;                       \
  THREADY_POSITION_NAME:                                                       \
  if (localThreadyVarContext->timestampDelay >= uptimeGetMs())                 \
    return;                                                                    \
  localThreadyVarContext->pos = localThreadyVarContext->posSleep;

#define threadyMacroRepeat()                                                   \
  localThreadyVarContext->repeatCount = 0;                                     \
  localThreadyVarContext->pos = &&THREADY_POSITION_NAME;                       \
  THREADY_POSITION_NAME:
#define threadyMacroCount(count)                                               \
  if (++localThreadyVarContext->repeatCount == (count)) {                      \
    localThreadyVarContext->pos = &&THREADY_POSITION_NAME;                     \
    goto THREADY_POSITION_NAME;                                                \
  } else                                                                       \
    return;                                                                    \
  THREADY_POSITION_NAME:                                                       \
  asm volatile("NOP");

#define threadyMacroUntilOrCount(condition, count)                             \
  if (++localThreadyVarContext->repeatCount == (count) || (condition)) {       \
    localThreadyVarContext->pos = &&THREADY_POSITION_NAME;                     \
    goto THREADY_POSITION_NAME;                                                \
  } else                                                                       \
    return;                                                                    \
  THREADY_POSITION_NAME:                                                       \
  if (localThreadyVarContext->repeatCount == (count))

/*
#define threadyMacroWaitUntil(condition)
#define threadyMacroWaitUntilOrTimeoutMs(condition, timeoutMs) //else can be
used to check timeout

#define threadyMacroRepeat(count)
#define threadyMacroRepeatUntil(condition, count) //else can be used to detect
failure
*/

#define threadyMacroCancel(code)                                               \
  localThreadyVarContext->status = THREADY_STATUS_CANCELLED;                   \
  localThreadyVarContext->exitCode = code;                                     \
  return;

#define threadyMacroYield()                                                    \
  localThreadyVarContext->pos = &&THREADY_POSITION_NAME;                       \
  return;                                                                      \
  THREADY_POSITION_NAME:                                                       \
  asm volatile("NOP")

#define threadyMacroSave()                                                     \
  localThreadyVarContext->savedPos = &&THREADY_POSITION_NAME;                  \
  THREADY_POSITION_NAME:                                                       \
  asm volatile("NOP")

#define threadyMacroRestore()                                                  \
  localThreadyVarContext->pos = localThreadyVarContext->savedPos;              \
  return;

#define threadyMacroGetId() localThreadyVarContext->id
#define threadyMacroGetLoopIndex() localThreadyVarContext->repeatCount
#define threadyMacroGetUserData() localThreadyVarContext->userData
#define threadyMacroGetUserDataSize() localThreadyVarContext->userDataSize

#endif /* THREADY_MACROS_H_ */
