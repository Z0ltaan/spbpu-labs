#include "queue.h"
#include <assert.h>
#include <stddef.h>

void
TaskQueueInit(TTaskQueue* queue)
{
  assert(queue);
  queue->head = NULL;
  queue->tail = NULL;
}

bool
IsTaskQueueEmpty(TTaskQueue* queue)
{
  assert(queue);
  return queue->head == NULL;
}

void
TaskQueuePush(TTaskQueue* queue, TTaskQueueNode* node)
{
  assert(queue);
  if (queue->head == NULL)
  {
    queue->tail = queue->head = node;
  }
  else
  {
    queue->tail = queue->tail->next = node;
  }
}

TTaskQueueNode*
TaskQueuePop(TTaskQueue* queue)
{
  assert(queue);

  TTaskQueueNode* head = queue->head;
  if (head == queue->tail)
  {
    queue->head = queue->tail = NULL;
  }
  else
  {
    queue->head = queue->head->next;
  }
  return head;
}
