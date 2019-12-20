#ifndef SPEAKER_HPP
#define SPEAKER_HPP

#define GB7_TIMER_USE_EVOKE

#include "port.hpp"
#include "timer.hpp"
#include "queue.hpp"

namespace gb7
{
    enum class Tone: long
    {
        None = 1000000000,
        C    = 3822,
        D    = 3405,
        E    = 3033,
        F    = 2863,
        G    = 2551,
        A    = 2272,
        B    = 2024,
    };

    class speaker
    {
        struct Note
        {
            Tone tone;
            long length; // milliseconds
        };

        static inline queue<Note> notes;
        static inline Tone tone = Tone::None;
        static inline long count = 0;
        static inline long count_to = 0;

    public:
        template<class SpeakerPin>
        static inline void init()
        {
            gb7::timer::timer0::init();

            using namespace gb7::timer::literals;
            gb7::timer::timer0::evoke_every(2000_us, on_timer<SpeakerPin>, nullptr);
        }

        static inline bool enqueue_note(Tone tone, long length)
        {
            return notes.push({ tone, length });
        }

        template<class SpeakerPin>
        static void on_timer(void*)
        {
            SpeakerPin pin;
            pin = !pin;
            return;

            if (count == count_to)
            {
                if (notes.size() > 0)
                {
                    Note note_temp;
                    notes.pop(note_temp);

                    tone = note_temp.tone;
                    count_to = 2 * note_temp.length / static_cast<long>(tone);
                    count = 0;

                    gb7::timer::timer0::evoke_in(gb7::timer::literals::operator""_us(static_cast<long>(tone) / 2), on_timer<SpeakerPin>);
                }
                else
                {
                    count_to = 0;
                    count = 0;

                    using namespace gb7::timer::literals;
                    gb7::timer::timer0::evoke_in(100_ms, on_timer<SpeakerPin>);
                }
            }
            else if(count < count_to)
            {
                count++;

                pin = !pin;
            }
        }
    };
}

#endif // SPEAKER_HPP
