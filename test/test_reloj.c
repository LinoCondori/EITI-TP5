#include "unity.h"
#include "clock.h"
/*
1) Configurar la libreria, consultar la hora y tiene que invalida.
2) Configurar la libreria, ajustar la hora (con valores correctos), consultar la hora y tiene que ser valida.
3) Configurar la libreria, ajustar la hora (con valores incorrectos) tiene que devolver un error y seguir siendo invalida.
4) Simular el paso de n ciclos de reloj, consultar la hora y verificar que avanzo un segundo.
5) Simular el paso de 10*n ciclos de reloj, consultar la hora y verificar que avanzo 10 segundos y varios igual.
6) Configurar la hora de la alarma (con valores correctos) y revisar si la guarda.
7) Configurar la hora de la alarma (con valores incorrectos) y revisar si la rechaza.
8) Configurar la hora de la alarma (con valores correctos) y revisar si queda activada.
9) Si la alarma esta activa y la desactivo, queda desactiva, pero no cambia la hora.
10) Si la alarma esta desactiva y la activo, queda activa, pero no cambia la hora.
11) Si la alarma esta activa, y la hora del reloj coincide con la hora de la alarma, entonces suena.
12) Si la alarma se activo y la pospongo n minutos, vuelve a sonar en n minutos.
13) Si la alarma se activo y la cancelo, vuelve a sonar en al dia siguiente.
14) 
*/

#define TICKS_PER_SECOND 5
clock_t reloj;

void SimulateSeconds(int seconds){
        for(int index = 0; index < TICKS_PER_SECOND * seconds; index++){
        ClockNewTick(reloj);
    }
}

void setUp(void){
    static const uint8_t INICIAL[] = {1, 2, 3, 4};
    reloj = ClockCreate(TICKS_PER_SECOND);
    ClockSetupTime(reloj, INICIAL, sizeof(INICIAL));

}

//1) Configurar la libreria, consultar la hora y tiene que invalida.
void test_hora_inicial_invalida(void) {
    static const uint8_t ESPERADO[] = {0, 0, 0, 0, 0, 0};
    uint8_t hora[6];
    reloj = ClockCreate(TICKS_PER_SECOND);
    TEST_ASSERT_FALSE(ClockGetTime(reloj, hora, sizeof(hora)));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, sizeof(ESPERADO));
}


//2) Configurar la libreria, ajustar la hora (con valores correctos), consultar la hora y tiene que ser valida.
void test_set_up_current_time(void){
    
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 0, 0};
    uint8_t hora[6];
    TEST_ASSERT_TRUE(ClockGetTime(reloj, hora, sizeof(hora)));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, sizeof(ESPERADO));
}

//4) Simular el paso de n ciclos de reloj, consultar la hora y verificar que avanzo un segundo.

void test_one_second_elapsed(void){

    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 0, 1};
    uint8_t hora[6];
    SimulateSeconds(1);
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, sizeof(ESPERADO));
}

void test_ten_second_elapsed(void){

    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 1, 0};
    uint8_t hora[6];
    SimulateSeconds(10);
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, sizeof(ESPERADO));
}