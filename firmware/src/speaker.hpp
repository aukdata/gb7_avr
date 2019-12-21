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
        None = -1,
        C    = 3822,
        D    = 3405,
        E    = 3033,
        F    = 2863,
        G    = 2551,
        A    = 2272,
        B    = 2024,
    };

    class speaker // consumes timer0
    {
        struct Note
        {
            Tone tone;
            uint64_t length; // microseconds
        };

        static inline queue<Note> m_notes;
        static inline Tone m_tone = Tone::None;
        static inline uint64_t m_count = 0;
        static inline uint64_t m_count_to = 0;

    public:
        template<class SpeakerPin>
        static inline void init()
        {
            gb7::timer::timer0::init();

            using namespace gb7::timer::literals;
            gb7::timer::timer0::evoke_every(100_ms, on_timer<SpeakerPin>);
        }

        static inline bool enqueue_note(Tone tone, uint64_t length)
        {
            return m_notes.push({ tone, length });
        }

        template<class SpeakerPin>
        static void on_timer(void*)
        {
            SpeakerPin pin;

            if(m_count < m_count_to)
            {
                m_count++;

                pin = !pin;
            }
            else // if (m_count >= m_count_to)
            {
                if (m_notes.size() > 0)
                {
                    Note note_temp;
                    m_notes.pop(note_temp);

                    if (note_temp.tone == Tone::None)
                    {
                        m_count = 0;
                        m_count_to = 0;

                        gb7::timer::timer0::evoke_every(gb7::timer::literals::operator""_us(note_temp.length / 2), on_timer<SpeakerPin>);
                    }
                    else
                    {
                        m_tone = note_temp.tone;
                        m_count_to = 2 * note_temp.length / static_cast<long>(m_tone);
                        m_count = 0;

                        gb7::timer::timer0::evoke_every(gb7::timer::literals::operator""_us(static_cast<long>(m_tone) / 2), on_timer<SpeakerPin>);
                    }
                }
                else
                {
                    m_count_to = 0;
                    m_count = 0;

                    using namespace gb7::timer::literals;
                    gb7::timer::timer0::evoke_in(100_ms, on_timer<SpeakerPin>);
                }
            }
        }
    };
}

#endif // SPEAKER_HPP
