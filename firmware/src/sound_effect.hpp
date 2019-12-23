#ifndef SOUND_EFFECT_HPP
#define SOUND_EFFECT_HPP

#include "speaker.hpp"

namespace gb7::sound
{
    class sound_effect // consumes timer0
    {
    public:
        template<class SpeakerPin>
        static void init()
        {
            speaker::init<SpeakerPin>();
        }

        static void stop()
        {
            speaker::stop_note();
        }

        enum class SoundType
        {
            Hit,
            Tulip,
        };
        static bool play(SoundType type)
        {
            switch (type)
            {
            case SoundType::Hit:
                speaker::enqueue_note(Tone::C, 0.1e6);
                speaker::enqueue_note(Tone::E, 0.1e6);
                speaker::enqueue_note(Tone::G, 0.1e6);
                return true;

            case SoundType::Tulip:
                speaker::enqueue_note(Tone::C    , 0.5e6);
                speaker::enqueue_note(Tone::D    , 0.5e6);
                speaker::enqueue_note(Tone::E    , 0.5e6);
                speaker::enqueue_note(Tone::None , 0.5e6);
                speaker::enqueue_note(Tone::C    , 0.5e6);
                speaker::enqueue_note(Tone::D    , 0.5e6);
                speaker::enqueue_note(Tone::E    , 0.5e6);
                speaker::enqueue_note(Tone::None , 0.5e6);

                speaker::enqueue_note(Tone::G    , 0.5e6);
                speaker::enqueue_note(Tone::E    , 0.5e6);
                speaker::enqueue_note(Tone::D    , 0.5e6);
                speaker::enqueue_note(Tone::C    , 0.5e6);
                speaker::enqueue_note(Tone::D    , 0.5e6);
                speaker::enqueue_note(Tone::E    , 0.5e6);
                speaker::enqueue_note(Tone::D    , 0.5e6);
                speaker::enqueue_note(Tone::None , 0.5e6);

                speaker::enqueue_note(Tone::C    , 0.5e6);
                speaker::enqueue_note(Tone::D    , 0.5e6);
                speaker::enqueue_note(Tone::E    , 0.5e6);
                speaker::enqueue_note(Tone::None , 0.5e6);
                speaker::enqueue_note(Tone::C    , 0.5e6);
                speaker::enqueue_note(Tone::D    , 0.5e6);
                speaker::enqueue_note(Tone::E    , 0.5e6);
                speaker::enqueue_note(Tone::None , 0.5e6);

                speaker::enqueue_note(Tone::G    , 0.5e6);
                speaker::enqueue_note(Tone::E    , 0.5e6);
                speaker::enqueue_note(Tone::D    , 0.5e6);
                speaker::enqueue_note(Tone::C    , 0.5e6);
                speaker::enqueue_note(Tone::D    , 0.5e6);
                speaker::enqueue_note(Tone::E    , 0.5e6);
                speaker::enqueue_note(Tone::C    , 0.5e6);
                speaker::enqueue_note(Tone::None , 0.5e6);

                speaker::enqueue_note(Tone::G    , 0.5e6);
                speaker::enqueue_note(Tone::None , 0.001e6);
                speaker::enqueue_note(Tone::G    , 0.5e6);
                speaker::enqueue_note(Tone::E    , 0.5e6);
                speaker::enqueue_note(Tone::G    , 0.5e6);
                speaker::enqueue_note(Tone::A    , 0.5e6);
                speaker::enqueue_note(Tone::None , 0.001e6);
                speaker::enqueue_note(Tone::A    , 0.5e6);
                speaker::enqueue_note(Tone::G    , 0.5e6);
                speaker::enqueue_note(Tone::None , 0.5e6);

                speaker::enqueue_note(Tone::E    , 0.5e6);
                speaker::enqueue_note(Tone::None , 0.001e6);
                speaker::enqueue_note(Tone::E    , 0.5e6);
                speaker::enqueue_note(Tone::D    , 0.5e6);
                speaker::enqueue_note(Tone::None , 0.001e6);
                speaker::enqueue_note(Tone::D    , 0.5e6);
                speaker::enqueue_note(Tone::Ch   , 1.0e6);
                return true;
            }
            return false;
        }
    };
} // namespace gb7::sound

#endif // SOUND_EFFECT_HPP
