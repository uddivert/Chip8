#include <pthread.h>
#include <unistd.h>
#include <ncurses.h>
#include <GL/glut.h>
#include "gui.h"
#include "debugger.h"


/*read keyboard and see gui at the same time*/
void *keyThread(void *vargp) 
{ 
    printf("This is the keyThread\n"); 
    return NULL; 
} 

int main1(int argc, char** argv)
{
	pthread_t thread_id;
	printf("Before Thread\n"); 
    pthread_create(&thread_id, NULL, keyThread, NULL); 
	guiInit(argc, argv);
    sleep(1); 
    pthread_join(thread_id, NULL); 
	return 0;
}
