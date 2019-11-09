#include <avr/io.h>

namespace gb7
{
    enum class Port
    {
        PortB, PortC, PortD,
    };
    
    template<Port P>
    class port
    {
    public:
        port() {}
        ~port() {}

        [[nodiscard]] constexpr volatile uint8_t* get_ddr_address() const
        {
            switch (P)
            {
            case Port::PortB:
                return &DDRB;
            case Port::PortC:
                return &DDRC;
            case Port::PortD:
                return &DDRD;
            }
        }
        [[nodiscard]] constexpr volatile uint8_t* get_port_address() const
        {
            switch (P)
            {
            case Port::PortB:
                return &PORTB;
            case Port::PortC:
                return &PORTC;
            case Port::PortD:
                return &PORTD;
            }
        }
        [[nodiscard]] constexpr volatile uint8_t* get_pin_address() const
        {
            switch (P)
            {
            case Port::PortB:
                return &PINB;
            case Port::PortC:
                return &PINC;
            case Port::PortD:
                return &PIND;
            }
        }
    };

    template<Port P>
    class port_readable: port<P>
    {
    public:
        port_readable(uint8_t pull_up = 0xff)
        {
            *(this->get_ddr_address()) = 0x00;
            if (pull_up != 0x00)
                *(this->get_port_address()) = pull_up;
        }
        ~port_readable() {}

        inline uint8_t read() const
        {
             return *(this->get_pin_address());
        }
    };

    template<Port P>
    class port_writable: port<P>
    {
    public:
        port_writable(uint8_t reversed = 0x00)
        {
            *(this->get_ddr_address()) = 0xff;
            if (reversed != 0x00)
                *(this->get_pin_address()) = reversed;
        }
        ~port_writable() {}

        inline uint8_t read() const
        {
            return *(this->get_port_address());
        }
        inline void write(uint8_t value) const
        {
            *(this->get_port_address()) = value;
        }
    };

    template<Port P, uint8_t N, port<P> p>
    class pin
    {
    private:
        volatile uint8_t* addr;
        uint8_t bit;

    public:
        pin();
        ~pin();
    };
    

} // namespace gb7
