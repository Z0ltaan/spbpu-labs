#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include "project_defs.h"

#define DeclareTask(TaskID) void TaskBody_##TaskID();
#define TASK(TaskID) void TaskBody_##TaskID()

// NOTE: priority will be changed to period some time later (not very
// significant)
int8_t
RegisterTask(TTask taskID, void (*taskFunc)(void), int8_t priority);

void
ActivateTask(TTask task);

void
TerminateTask();

void
YeildTask();

// TODO: smth about 16 events per task
// 1. wtf is event
// 2. how do we use that shit in tasks

#endif
