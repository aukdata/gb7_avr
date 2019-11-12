#include <avr/interrupt.h>

#include "timer.hpp"

ISR(TIMER2_OVF_vect)
{
    gb7::timer::multi_timer::on_timer_interrupt();
}
