#ifndef TIMER_HPP
#define TIMER_HPP

#include <avr/io.h>

namespace gb7
{
    using time_unit = uint32_t;

    namespace timer
    {
        namespace raw_timers
        {
            enum class pwm_mode
            {
                none            = 0b00,
                toggle_on_match = 0b01,
                low_on_match    = 0b10,
                high_on_match   = 0b11,
            };
            enum class timer_mode
            {
                normal            = 0b00,
                ctc               = 0b10,
                fast_pwm          = 0b11,
                phase_correct_pwm = 0b01,
            };
            enum class clock_division
            {
                no_clock              = 0b000,
                no_division           = 0b001,
                division_8            = 0b010,
                division_64           = 0b011,
                division_256          = 0b100,
                division_1024         = 0b101,
                external_falling_edge = 0b110,
                external_rising_edge  = 0b111,
            };

            class timer_address_converter
            {

            };

            class timer0
            {
            public:
                timer0() = delete;
                ~timer0() = delete;

                static void init(pwm_mode oc0a, pwm_mode oc0b, timer_mode mode, clock_division division) noexcept
                {
                    TCCR0A =
                        (static_cast<uint8_t>(oc0a) << 6) |
                        (static_cast<uint8_t>(oc0b) << 4) |
                        (static_cast<uint8_t>(mode));
                    TCCR0B = static_cast<uint8_t>(division);
                }

                static void enable_timer_interrupt(bool compare_a, bool compare_b, bool overflow) noexcept
                {
                    if (compare_a || compare_b || overflow)
                    {
                        TIMSK0 =
                            (compare_a ? 0b010 : 0) |
                            (compare_b ? 0b110 : 0) |
                            (overflow  ? 0b001 : 0);
                    }
                }
            };

            class timer2
            {
            public:
                timer2() = delete;
                ~timer2() = delete;

                inline static void init(pwm_mode oc0a, pwm_mode oc0b, timer_mode mode, clock_division division) noexcept
                {
                    TCCR2A =
                        (static_cast<uint8_t>(oc0a) << 6) |
                        (static_cast<uint8_t>(oc0b) << 4) |
                        (static_cast<uint8_t>(mode));
                    TCCR2B = static_cast<uint8_t>(division);
                }

                inline static void enable_timer_interrupt(bool compare_a, bool compare_b, bool overflow) noexcept
                {
                    if (compare_a || compare_b || overflow)
                    {
                        TIMSK2 =
                            (compare_a ? 0b010 : 0) |
                            (compare_b ? 0b110 : 0) |
                            (overflow  ? 0b001 : 0);
                    }
                }
            };
        } // namespace raw_timers

        namespace literals
        {
            constexpr time_unit operator""_us(unsigned long long v) noexcept
            {
                return static_cast<time_unit>(v);
            }
            constexpr time_unit operator""_ms(unsigned long long v) noexcept
            {
                return static_cast<time_unit>(v * 1e3);
            }
            constexpr time_unit operator""_s(unsigned long long v) noexcept
            {
                return static_cast<time_unit>(v * 1e6);
            }
        } // namespace literals


        // consume timer 2
        class multi_timer
        {
        public:
            using callback_func = void(*)(void*);

        private:
            struct stack_item
            {
            private:
                inline static bool initialized = false;

            public:
                multi_timer() noexcept
                {
                    raw_timers::timer2::init(
                        raw_timers::pwm_mode::none,
                        raw_timers::pwm_mode::none,
                        raw_timers::timer_mode::normal,
                        raw_timers::clock_division::division_8
                    );
                    raw_timers::timer2::enable_timer_interrupt(false, false, true);
                }
                ~multi_timer()
                {

                }
            };

            template<time_unit us, class function>
            void evoke_in(function callback)
            {
            }

            template<time_unit us, class function>
            void evoke_every(function callback)
            {
            }
        };
    }
} // namespace gb7

#endif // TIMER_HPP
