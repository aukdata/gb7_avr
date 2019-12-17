#ifndef TIMER_HPP
#define TIMER_HPP

#include <avr/io.h>

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

            inline static void set_ctc_top(uint8_t ocr0a) noexcept
            {
                OCR0A = ocr0a;
            }

            static void enable_timer_interrupt(bool compare_a, bool compare_b, bool overflow) noexcept
            {
                if (compare_a || compare_b || overflow)
                {
                    TIMSK0 =
                        (compare_a ? 0b010 : 0) |
                        (compare_b ? 0b100 : 0) |
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
        constexpr inline static double TIME_COEFF = 1e-6 / (1024 / static_cast<double>(F_CPU));
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


    class multi_timer
    {
    public:
        using callback_func = void(*)(void*);

    private:
        struct stack_item
        {
            callback_func func;
            void* data;
            time_unit next_time;
            time_unit period;
        };

        constexpr inline static int MAX_CALLBACK_FUNC_NUM = 8;

        inline static stack_item items[MAX_CALLBACK_FUNC_NUM];
        inline static time_unit now = 0;

    public:
        multi_timer() = delete;
        ~multi_timer() = delete;

        static void init() noexcept
        {
            raw_timers::timer2::init(
                raw_timers::pwm_mode::none, raw_timers::pwm_mode::none,
                raw_timers::timer_mode::normal, raw_timers::clock_division::no_division
            );
            for (int i = 0; i < MAX_CALLBACK_FUNC_NUM; i++)
            {
                items[i].func = nullptr;
            }

            raw_timers::timer2::enable_overflow_interrupt();
            sei();
        }


        template<time_unit time>
        static int evoke_in(callback_func callback, void* data) noexcept
        {
            for (int i = 0; i < MAX_CALLBACK_FUNC_NUM; i++)
            {
                if (items[i].func == nullptr)
                {
                    items[i].func = callback;
                    items[i].data = data;
                    items[i].next_time = now + time;
                    items[i].period = 0;
                    return i;
                }
            }
            return -1;
        }

        template<time_unit time>
        static int evoke_every(callback_func callback, void* data) noexcept
        {
            for (int i = 0; i < MAX_CALLBACK_FUNC_NUM; i++)
            {
                if (items[i].func == nullptr)
                {
                    items[i].func = callback;
                    items[i].data = data;
                    items[i].next_time = now + time;
                    items[i].period = time;
                    return i;
                }
            }
            return -1;
        }

        static void remove_callback(int id) noexcept
        {
            if (0 <= id && id < MAX_CALLBACK_FUNC_NUM)
                items[id].func = nullptr;
        }

        static void on_timer_interrupt() noexcept
        {
            cli();
            now++;
            for (int i = 0; i < MAX_CALLBACK_FUNC_NUM; i++)
            {
                if (items[i].next_time == now && items[i].func != nullptr)
                {
                    items[i].func(items[i].data);
                    if (items[i].period > 0)
                    {
                        items[i].next_time += items[i].period;
                    }
                    else
                    {
                        items[i].func = nullptr;
                    }
                }
            }
            sei();
        }
    };
} // namespace gb7::timer

ISR(TIMER2_OVF_vect)
{
    gb7::timer::multi_timer::on_timer_interrupt();
}

#endif // TIMER_HPP
