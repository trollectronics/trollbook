#ifndef SOUND_H_
#define SOUND_H_

void sound_setup(void *buf_addr);
void sound_start();
void sound_stop();
int sound_wait();

#endif
