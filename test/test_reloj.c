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
bool alarm_state;

void SimulateSeconds(int seconds){
        for(int index = 0; index < TICKS_PER_SECOND * seconds; index++){
        ClockNewTick(reloj);
    }
}

void AlarmEventHandler(clock_t clock, bool state){
    alarm_state = state;
}

void setUp(void){
    static const uint8_t INICIAL[] = {1, 2, 3, 4};
    reloj = ClockCreate(TICKS_PER_SECOND, AlarmEventHandler);
    ClockSetupTime(reloj, INICIAL, sizeof(INICIAL));
    alarm_state = false;

}

//1) Configurar la libreria, consultar la hora y tiene que invalida.
void test_hora_inicial_invalida(void) {
    static const uint8_t ESPERADO[] = {0, 0, 0, 0, 0, 0};
    uint8_t hora[6];
    uint8_t alarma[4];
    reloj = ClockCreate(TICKS_PER_SECOND, AlarmEventHandler);
    TEST_ASSERT_FALSE(ClockGetTime(reloj, hora, sizeof(hora)));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, sizeof(ESPERADO));
    TEST_ASSERT_FALSE(ClockGetAlarm(reloj, alarma, sizeof(alarma)));
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

void test_one_minute_elapsed(void){

    static const uint8_t ESPERADO[] = {1, 2, 3, 5, 0, 0};
    uint8_t hora[6];
    SimulateSeconds(60);
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, sizeof(ESPERADO));
}

void test_ten_minute_elapsed(void){

    static const uint8_t ESPERADO[] = {1, 2, 4, 4, 0, 0};
    uint8_t hora[6];
    SimulateSeconds(60 * 10);
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, sizeof(ESPERADO));
}

void test_one_hour_elapsed(void){

    static const uint8_t ESPERADO[] = {1, 3, 3, 4, 0, 0};
    uint8_t hora[6];
    SimulateSeconds(60 * 60);
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, sizeof(ESPERADO));
}
void test_ten_hour_elapsed(void){

    static const uint8_t ESPERADO[] = {2, 2, 3, 4, 0, 0};
    uint8_t hora[6];
    SimulateSeconds(60 * 60 * 10);
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, sizeof(ESPERADO));
}
void test_one_day_elapsed(void){

    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 0, 0};
    uint8_t hora[6];
    SimulateSeconds(60 * 60 * 24 );
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, sizeof(ESPERADO));
}

void test_setup_and_get_alarm(void){
    static const uint8_t ALARMA[] = {1, 2, 3, 5};
    uint8_t hora[4];

    ClockSetupAlarm(reloj, ALARMA, sizeof(ALARMA));
    TEST_ASSERT_TRUE(ClockGetAlarm(reloj, hora, sizeof(hora)));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ALARMA, hora, sizeof(ALARMA));
}

void test_setup_and_disable_alarm(void){
    static const uint8_t ALARMA[] = {1, 2, 3, 5};
    uint8_t hora[4];

    ClockSetupAlarm(reloj, ALARMA, sizeof(ALARMA));
    TEST_ASSERT_FALSE(ClockToggleAlarm(reloj));

    TEST_ASSERT_FALSE(ClockGetAlarm(reloj, hora, sizeof(hora)));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ALARMA, hora, sizeof(ALARMA));
}

void test_setup_and_fire_alarm(void){
    static const uint8_t ALARMA[] = {1, 2, 3, 5};
    ClockSetupAlarm(reloj, ALARMA, sizeof(ALARMA));
    SimulateSeconds(60);
    TEST_ASSERT_TRUE(alarm_state);
    alarm_state = false;
    SimulateSeconds(60); //24*60*60-1
    TEST_ASSERT_FALSE(alarm_state);

}

void test_setup_and_not_fire_alarm(void){
    static const uint8_t ALARMA[] = {1, 2, 3, 5};

    ClockSetupAlarm(reloj, ALARMA, sizeof(ALARMA));
    ClockToggleAlarm(reloj);

    SimulateSeconds(60);
    TEST_ASSERT_FALSE(alarm_state);

}





