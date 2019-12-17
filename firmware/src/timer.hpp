#ifndef TIMER_HPP
#define TIMER_HPP

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef F_CPU
#define F_CPU 8000000
#endif // F_CPU


namespace gb7::timer
{
    using time_unit = uint64_t;

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
        enum class timer_top
        {
            ff   = 0b0,
            ocra = 0b1,
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

        class raw_timer0
        {
        public:
            raw_timer0() = delete;
            ~raw_timer0() = delete;

            static void init(pwm_mode oc0a, pwm_mode oc0b, timer_mode mode, timer_top top, clock_division division) noexcept
            {
                TCCR0A =
                    (static_cast<uint8_t>(oc0a) << 6) |
                    (static_cast<uint8_t>(oc0b) << 4) |
                    (static_cast<uint8_t>(mode));
                TCCR0B =
                    (static_cast<uint8_t>(top) << 3) |
                    (static_cast<uint8_t>(division));
            }

            inline static void set_ctc_top(uint8_t ocr0a) noexcept
            {
                OCR0A = ocr0a;
            }

            inline static void enable_compare_match_a_interrupt(uint8_t count) noexcept
            {
                TIMSK0 |= 0b010;
                OCR0A = count;
            }
            inline static void enable_compare_match_b_interrupt(uint8_t count) noexcept
            {
                TIMSK0 |= 0b100;
                OCR0B = count;
            }
            inline static void enable_overflow_interrupt() noexcept
            {
                TIMSK0 |= 0b001;
            }
        };

        class raw_timer2
        {
        public:
            raw_timer2() = delete;
            ~raw_timer2() = delete;

            static void init(pwm_mode oc0a, pwm_mode oc0b, timer_mode mode, timer_top top, clock_division division) noexcept
            {
                TCCR2A =
                    (static_cast<uint8_t>(oc0a) << 6) |
                    (static_cast<uint8_t>(oc0b) << 4) |
                    (static_cast<uint8_t>(mode));
                TCCR2B =
                    (static_cast<uint8_t>(top) << 3) |
                    (static_cast<uint8_t>(division));
            }

            inline static void set_ctc_top(uint8_t ocr2a) noexcept
            {
                OCR2A = ocr2a;
            }

            inline static void enable_compare_match_a_interrupt(uint8_t count) noexcept
            {
                TIMSK2 |= 0b010;
                OCR2A = count;
            }
            inline static void enable_compare_match_b_interrupt(uint8_t count) noexcept
            {
                TIMSK2 |= 0b100;
                OCR2B = count;
            }
            inline static void enable_overflow_interrupt() noexcept
            {
                TIMSK2 |= 0b001;
            }
        };
    } // namespace raw_timers


    namespace literals
    {
        constexpr inline static double TIME_COEFF = 1e-6 / (256 / static_cast<double>(F_CPU));
        constexpr time_unit operator""_us(unsigned long long v) noexcept
        {
            return static_cast<time_unit>(v * TIME_COEFF);
        }
        constexpr time_unit operator""_ms(unsigned long long v) noexcept
        {
            return static_cast<time_unit>(v * TIME_COEFF * 1e3);
        }
        constexpr time_unit operator""_s(unsigned long long v) noexcept
        {
            return static_cast<time_unit>(v * TIME_COEFF * 1e6);
        }
    } // namespace literals


    using callback_func = void(*)(void*);

    class timer0
    {
        static inline callback_func callback = nullptr;
        static inline void* data = nullptr;
        static inline time_unit next_time = 0;
        static inline time_unit period = 0;

        static inline time_unit now = 0;

    public:
        timer0() = delete;
        ~timer0() = delete;

        static void init() noexcept
        {
            raw_timers::raw_timer0::init(
                raw_timers::pwm_mode::none, raw_timers::pwm_mode::none, raw_timers::timer_mode::normal,
                raw_timers::timer_top::ff, raw_timers::clock_division::no_division
            );

            raw_timers::raw_timer0::enable_overflow_interrupt();
            sei();
        }

        template<time_unit time>
        static void evoke_in(callback_func f, void* d) noexcept
        {
            callback = f;
            data = d;
            next_time = now + time;
            period = 0;
        }

        template<time_unit time>
        static void evoke_every(callback_func f, void* d) noexcept
        {
            callback = f;
            data = d;
            next_time = now + time;
            period = time;
        }

        static void on_timer_interrupt() noexcept
        {
            cli();

            now++;
            if (now == next_time && callback != nullptr)
            {
                callback(data);
                if (period > 0)
                {
                    next_time += period;
                }
                else
                {
                    callback = nullptr;
                }
            }

            sei();
        }
    };

    class timer2
    {
        static inline callback_func callback = nullptr;
        static inline void* data = nullptr;
        static inline time_unit next_time = 0;
        static inline time_unit period = 0;

        static inline time_unit now = 0;

    public:
        timer2() = delete;
        ~timer2() = delete;

        static void init() noexcept
        {
            raw_timers::raw_timer2::init(
                raw_timers::pwm_mode::none, raw_timers::pwm_mode::none, raw_timers::timer_mode::normal,
                raw_timers::timer_top::ff, raw_timers::clock_division::no_division
            );

            raw_timers::raw_timer2::enable_overflow_interrupt();
            sei();
        }

        template<time_unit time>
        static void evoke_in(callback_func f, void* d) noexcept
        {
            callback = f;
            data = d;
            next_time = now + time;
            period = 0;
        }

        template<time_unit time>
        static void evoke_every(callback_func f, void* d) noexcept
        {
            callback = f;
            data = d;
            next_time = now + time;
            period = time;
        }

        static void on_timer_interrupt() noexcept
        {
            cli();

            now++;
            if (now == next_time && callback != nullptr)
            {
                callback(data);
                if (period > 0)
                {
                    next_time += period;
                }
                else
                {
                    callback = nullptr;
                }
            }

            sei();
        }
    };
} // namespace gb7::timer

ISR(TIMER0_OVF_vect)
{
    gb7::timer::timer0::on_timer_interrupt();
}
ISR(TIMER2_OVF_vect)
{
    gb7::timer::timer2::on_timer_interrupt();
}

#endif // TIMER_HPP
