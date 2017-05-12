//load mbed libraries, rots is needed for multithreading, queue for queue object

#include "mbed.h"
#include "rtos.h"
#include "string.h"
#include "queue.h"
#include "BufferedSerial.h"

//MAXNEMEAMSG is the max lenght of a valid NMEA message
#define MAXNMEAMSG 83

//tx pin first then rx 

BufferedSerial serDue(PA_2, PA_3);
BufferedSerial serUno(PA_15, PB_7);
BufferedSerial serSei(PA_11, PA_12);
::Queue myQueue( MAXNMEAMSG, 100 );

void trasmetti(void const *args)
{
    int i,l;
    while(1) {
        char messaggioTX[MAXNMEAMSG];

        if(myQueue.GetNumberOfItems()) {
            myQueue.Get(messaggioTX);
            //printf("%s", messaggioTX);
            printf("TX: %d \n\r",myQueue.GetNumberOfItems());
            l= strlen(messaggioTX);
            serSei.puts(messaggioTX);
        } else Thread::wait(50);

    }

}

void NMEARx(void const *args)
{

    char messaggio[MAXNMEAMSG];
    int indice;
    BufferedSerial* ps = (BufferedSerial*) args;
    int contatore=0;
    while (1) {
                indice =0;
                while (!ps->readable()) Thread::wait(10);
                messaggio[indice] = ps->getc(); //build the message
                if (messaggio[0] == '$' || messaggio[0] == '!') {
                    indice =1;
                    while (1) {
                        if (indice >= MAXNMEAMSG)  { //check is NMEA msg is too long wrt its specs
                            indice = 0;
                            break;
                        }
                        while (!ps->readable()) Thread::wait(10);
                        messaggio[indice++] = ps->getc(); // load next char into msg
                        if (messaggio[indice-1] == 0x0a)  { //newline
                            messaggio[indice] = '\0'; // null, string terminator
                            break;
                        }
                    } // while interno 1
                if (indice) {
                    // test things
                        // printf("Messaggio %s", messaggio);
                        //serSei.puts(messaggio);
                        //sprintf(messaggio,"%s %04d",messaggio,contatore++);
                    myQueue.Put(messaggio);
                    indice=0;
                        //sprintf(messaggio, "Ciao");
                }
                    // a 10 milliseconds pause can be added if needed
                    //Thread::wait(10);
        }

    }
}

int main()
{
    serUno.baud(4800);
    serSei.baud(115200);
    //NMEARx("");

    Thread thread_leggiGPS(NMEARx,&serUno);
    Thread thread_leggiAIS(NMEARx,&serDue);
    Thread thread_scrivi(trasmetti);
    while(1) {
        Thread::wait(1000);

    }

}
