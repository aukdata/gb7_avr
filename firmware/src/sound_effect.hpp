#ifndef SOUND_EFFECT_HPP
#define SOUND_EFFECT_HPP

#include "speaker.hpp"

namespace gb7::sound
{
    enum class SoundEffectType
    {
        Hit,
        Tulip,
    };

    template<class SpeakerPin>
    class sound_effect
    {
        speaker<SpeakerPin> sp;

    public:
        void init()
        {
            sp.init();
        }

        void stop()
        {
            sp.stop_note();
        }

        bool play(SoundEffectType type)
        {
            switch (type)
            {
            case SoundEffectType::Hit:
                sp.enqueue_note(Tone::C, 0.1e6);
                sp.enqueue_note(Tone::E, 0.1e6);
                sp.enqueue_note(Tone::G, 0.1e6);
                return true;

            case SoundEffectType::Tulip:
                sp.enqueue_note(Tone::C    , 0.5e6);
                sp.enqueue_note(Tone::D    , 0.5e6);
                sp.enqueue_note(Tone::E    , 0.5e6);
                sp.enqueue_note(Tone::None , 0.5e6);
                sp.enqueue_note(Tone::C    , 0.5e6);
                sp.enqueue_note(Tone::D    , 0.5e6);
                sp.enqueue_note(Tone::E    , 0.5e6);
                sp.enqueue_note(Tone::None , 0.5e6);

                sp.enqueue_note(Tone::G    , 0.5e6);
                sp.enqueue_note(Tone::E    , 0.5e6);
                sp.enqueue_note(Tone::D    , 0.5e6);
                sp.enqueue_note(Tone::C    , 0.5e6);
                sp.enqueue_note(Tone::D    , 0.5e6);
                sp.enqueue_note(Tone::E    , 0.5e6);
                sp.enqueue_note(Tone::D    , 0.5e6);
                sp.enqueue_note(Tone::None , 0.5e6);

                sp.enqueue_note(Tone::C    , 0.5e6);
                sp.enqueue_note(Tone::D    , 0.5e6);
                sp.enqueue_note(Tone::E    , 0.5e6);
                sp.enqueue_note(Tone::None , 0.5e6);
                sp.enqueue_note(Tone::C    , 0.5e6);
                sp.enqueue_note(Tone::D    , 0.5e6);
                sp.enqueue_note(Tone::E    , 0.5e6);
                sp.enqueue_note(Tone::None , 0.5e6);

                sp.enqueue_note(Tone::G    , 0.5e6);
                sp.enqueue_note(Tone::E    , 0.5e6);
                sp.enqueue_note(Tone::D    , 0.5e6);
                sp.enqueue_note(Tone::C    , 0.5e6);
                sp.enqueue_note(Tone::D    , 0.5e6);
                sp.enqueue_note(Tone::E    , 0.5e6);
                sp.enqueue_note(Tone::C    , 0.5e6);
                sp.enqueue_note(Tone::None , 0.5e6);

                sp.enqueue_note(Tone::G    , 0.5e6);
                sp.enqueue_note(Tone::None , 0.001e6);
                sp.enqueue_note(Tone::G    , 0.5e6);
                sp.enqueue_note(Tone::E    , 0.5e6);
                sp.enqueue_note(Tone::G    , 0.5e6);
                sp.enqueue_note(Tone::A    , 0.5e6);
                sp.enqueue_note(Tone::None , 0.001e6);
                sp.enqueue_note(Tone::A    , 0.5e6);
                sp.enqueue_note(Tone::G    , 0.5e6);
                sp.enqueue_note(Tone::None , 0.5e6);

                sp.enqueue_note(Tone::E    , 0.5e6);
                sp.enqueue_note(Tone::None , 0.001e6);
                sp.enqueue_note(Tone::E    , 0.5e6);
                sp.enqueue_note(Tone::D    , 0.5e6);
                sp.enqueue_note(Tone::None , 0.001e6);
                sp.enqueue_note(Tone::D    , 0.5e6);
                sp.enqueue_note(Tone::Ch   , 1.0e6);
                return true;
            }
            return false;
        }
    };
} // namespace gb7::sound

#endif // SOUND_EFFECT_HPP
