#include <stdlib.h>
#include <stdio.h>
#include "train.h"

struct track {
  track_t *next; 
  track_t *swch; 
  bool indicator; 
};

track_t *track_create(track_t *next) {
  track_t *trck = calloc(1, sizeof(track_t)); 
  trck->next = next; 
  return trck;
}

void track_destroy(track_t *track) {

  
  while (track != NULL)
  {
    if (track->swch != NULL)
    {
      track_destroy(track->swch); 
    }
    track_t *tmp = track;
    track = tmp->next;
    free(tmp);
  }
  return;
}

bool track_add_switch(track_t *from, track_t *to) {

  if (from->swch == NULL)
  {
    from->swch = to; 
    return true; 
  }

  return false;
}

bool track_has_train(track_t *track) {
  return track->indicator;
}

track_t *track_move(track_t *start, size_t steps, bool take_switch) {
  start->indicator = false; 
  for (size_t i = 0; i < steps; i++)
  {
    if (start->next == NULL)
    {
      start->indicator = true; 
      return start; 
    }
    
    if (take_switch && start->swch != NULL) 
    {
      if (start->swch->indicator)
      {
        break; 
      }
      
      start = start->swch;
      continue;
    }

    if (start->next->indicator)
    {
      break; 
    }
    
    start = start->next; 
  }
  start->indicator = true;
  return start;
}

#ifndef TEST
#include <stdio.h>

int main(void)
{
  puts("If you want to you can write test code here and run it using `make run`");
  return 0;
}
#endif
