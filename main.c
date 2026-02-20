#include "interrupts.h"
#include "serial.h"
#include "gpio.h"
#include "timer1.h"
#include "protocol.h"
#include "display.h"
#include "utils.h"

#define MOTOR P2_0
#define NITNA P0_6

/* ---------- debounce ---------- */
uint8_t taster_stabilno = 0x0F;
uint8_t taster_prethodno = 0x0F;
uint8_t debounce_cnt[4] = {0};
uint8_t dogadjajP0 = 0;

void debounce_tastera(void){
    uint8_t trenutno = P0 & 0x0F;
    uint8_t i;
    for(i=0;i<4;i++){
        uint8_t mask = 1<<i;
        if((trenutno&mask) == (taster_prethodno&mask)){
            if(debounce_cnt[i] < 5){
                debounce_cnt[i]++;
            } else {
                if((taster_stabilno&mask) != (trenutno&mask)){
                    taster_stabilno ^= mask;
                    if((taster_stabilno&mask) == 0){
                        dogadjajP0 |= mask;
                    }
                }
            }
        } else {
            debounce_cnt[i] = 0;
        }
    }
    taster_prethodno = trenutno;
}

/* ---------- MAIN ---------- */
void main(void){
    uint8_t start = 0;
    uint8_t punjenje = 0;
    uint8_t puni = 0;
    uint8_t nemaNitne = 0;
    uint8_t kvar = 0;
    uint8_t brojNapunjenih = 0;
    uint8_t trenutnoStanjeNitne = 0;
    uint8_t prethodnoStanjeNitne = 0;
    uint8_t total_stop = 0;
    char buf[5];

    Serial_Init(1,0,253);
    GlobalIT_enable(1);
    SerialIT_enable(1);
    init_gpio();
    timer1_init(0x01,0x00,0xEE,1);
    timer1_start();
    initDisplay();
    P2 = 0;

    while(1){

        /* ---------- debounce uvek ---------- */
        if(tick5ms){
            tick5ms = 0;
            debounce_tastera();
        }

        /* ---------- TOTAL STOP ---------- */
        if(total_stop){
            MOTOR = 0;
            P2_1 = 0;
            punjenje = 0;
            puni = 0;
            start = 0;

            if(program == 10 || (dogadjajP0 & (1<<0))){
                dogadjajP0 &= ~(1<<0);
                program = 0;
                total_stop = 0;
                if(!kvar){
                    start = 1;
                }
            }

            if((P0_2 == 0) || (dogadjajP0 & (1<<2))){
                dogadjajP0 = 0;
                kvar = 0;
                nemaNitne = 0;
                P2_2 = 0;
            }

            continue;
        }

        /* ---------- senzor nitne ---------- */
        trenutnoStanjeNitne = NITNA;
        if((trenutnoStanjeNitne == 1) && (prethodnoStanjeNitne == 0) && !kvar){
            punjenje = 1;
            start = 0;
            P2_1 = 1;
        }
        prethodnoStanjeNitne = trenutnoStanjeNitne;

        /* ---------- START ---------- */
        if(program == 10 || (dogadjajP0 & (1<<0))){
            dogadjajP0 &= ~(1<<0);
            program = 0;
            if(!kvar){
                start = 1;
            }
        }

        /* ---------- STOP ---------- */
        if(program == 11 || (dogadjajP0 & (1<<1))){
            dogadjajP0 &= ~(1<<1);
            program = 0;
            start = 0;
            total_stop = 1;
        }

        /* ---------- reset kvara ---------- */
        if((P0_2 == 0) || (dogadjajP0 & (1<<2))){
            dogadjajP0 = 0;
            kvar = 0;
            nemaNitne = 0;
            P2_2 = 0;
        }

        /* ---------- reset broja flasa ---------- */
        if((P0_3 == 0) || (dogadjajP0 & (1<<3))){
            dogadjajP0 = 0;
            brojNapunjenih = 0;
        }

        /* ---------- nema nitne 7 s ---------- */
        if(start && prosla1sekunda){
            prosla1sekunda = 0;
            nemaNitne++;
        }

        if(nemaNitne >= 7){
            kvar = 1;
            start = 0;
            P2_2 = 1;
        }

        /* ---------- punjenje ---------- */
        if(punjenje && prosla1sekunda){
            prosla1sekunda = 0;
            puni++;
            if(puni >= vreme_punjenja){
                puni = 0;
                punjenje = 0;
                brojNapunjenih++;
                start = 1;
                P2_1 = 0;
            }
        }

        /* ---------- motor ---------- */
        MOTOR = (start && !kvar && !punjenje) ? 1 : 0;

        /* ---------- display ---------- */
        if(prosla1sekunda){
            prosla1sekunda = 0;
            int_to_str(brojNapunjenih, buf);
            clearDisplay();
            writeLine("FLASE:");
            writeLine(buf);
            newLine();
            writeLine(kvar ? "KVAR: DA" : "KVAR: NE");
        }
    }
}
