#include "test/rtos_config.h"

int main()
{
  // unix posix thread_local
  handle_communication();
  pthread_t tid;
  pthread_create(&tid, NULL, handle_communication(), NULL);
  pthread_join(tid, NULL);
  
  return 0;
}
