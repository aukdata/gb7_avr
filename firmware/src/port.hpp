#ifndef PORT_H
#define PORT_H

#include <avr/io.h>

namespace gb7
{
    enum class port_type
    {
        PortB, PortC, PortD,
    };
    using pin_number = uint8_t;
    
    /*
     * declaration
     */
    template<port_type P>
    class port;

    template<port_type P>
    class port_readable;

    template<port_type P>
    class port_writable;

    template<port_type P, pin_number N>
    class pin_readable;

    template<port_type P, pin_number N>
    class pin_writable;


    /*
     * definition
     */
    template<port_type P>
    class port
    {
    public:
        port() noexcept {}
        ~port() = default;

        [[nodiscard]] constexpr volatile uint8_t* get_ddr_address() const noexcept
        {
            switch (P)
            {
            case port_type::PortB:
                return &DDRB;
            case port_type::PortC:
                return &DDRC;
            case port_type::PortD:
                return &DDRD;
            }
        }
        [[nodiscard]] constexpr volatile uint8_t* get_port_address() const noexcept
        {
            switch (P)
            {
            case port_type::PortB:
                return &PORTB;
            case port_type::PortC:
                return &PORTC;
            case port_type::PortD:
                return &PORTD;
            }
        }
        [[nodiscard]] constexpr volatile uint8_t* get_pin_address() const noexcept
        {
            switch (P)
            {
            case port_type::PortB:
                return &PINB;
            case port_type::PortC:
                return &PINC;
            case port_type::PortD:
                return &PIND;
            }
        }
    };

    template<port_type P>
    class port_readable: port<P>
    {
    public:
        port_readable(uint8_t pull_up = 0xff) noexcept
        {
            *(this->get_ddr_address()) = 0x00;
            if (pull_up != 0x00)
                *(this->get_port_address()) = pull_up;
        }
        ~port_readable() = default;

        inline uint8_t read() const noexcept
        {
             return *(this->get_pin_address());
        }
    };

    template<port_type P>
    class port_writable: port<P>
    {
    public:
        port_writable(uint8_t reversed = 0x00) noexcept
        {
            *(this->get_ddr_address()) = 0xff;
            if (reversed != 0x00)
                *(this->get_pin_address()) = reversed;
        }
        ~port_writable() = default;

        inline uint8_t read() const noexcept
        {
            return *(this->get_port_address());
        }
        inline void write(uint8_t value) const noexcept
        {
            *(this->get_port_address()) = value;
        }

        template<pin_number N>
        inline auto get_writable_pin() noexcept
        {
            return pin_writable<P, N> { *this };
        }
    };

    enum pin_io_config: uint8_t { readable = 0, writable = 1 };
    template<port_type P, const pin_io_config config[8]>
    class port_mixed: port<P>
    {
    private:
        inline static constexpr uint8_t mask = 
            (config[0] == pin_io_config::writable ? 1 : 0) << 0 |
            (config[1] == pin_io_config::writable ? 1 : 0) << 1 |
            (config[2] == pin_io_config::writable ? 1 : 0) << 2 |
            (config[3] == pin_io_config::writable ? 1 : 0) << 3 |
            (config[4] == pin_io_config::writable ? 1 : 0) << 4 |
            (config[5] == pin_io_config::writable ? 1 : 0) << 5 |
            (config[6] == pin_io_config::writable ? 1 : 0) << 6 |
            (config[7] == pin_io_config::writable ? 1 : 0) << 7;

    public:
        port_mixed() noexcept {
            *(this->get_ddr_address()) = mask;
        }
        ~port_mixed() = default;

        template<pin_number N>
        auto get_readable_pin() noexcept
        {
            static_assert(config[N] == pin_io_config::readable, "Tried to read non-readable pin!");
            return pin_readable<P, N> { *this };
        }

        template<pin_number N>
        auto get_writable_pin() noexcept
        {
            static_assert(config[N] == pin_io_config::writable, "Tried to write to non-writable pin!");
            return pin_writable<P, N> { *this };
        }
    };

    template<port_type P, pin_number N>
    class pin_readable
    {
    private:
        inline static constexpr uint8_t mask = (1 << N);
        const port<P>& p;

    public:
        pin_readable(port<P>& _p): p(_p) {}
        ~pin_readable() = default;

        inline bool read() const noexcept
        {
            return (*(p.get_pin_address()) & mask) != 0;
        }

        inline operator bool() const noexcept
        {
            return read();
        }
    };

    template<port_type P, pin_number N>
    class pin_writable
    {
    private:
        inline static constexpr uint8_t mask = (1 << N);
        const port<P>& p;

    public:
        pin_writable(port<P>& _p): p(_p) {}
        ~pin_writable() = default;

        inline bool read() const noexcept
        {
            return (*(p.get_port_address()) & mask) != 0;
        }

        inline void write(bool value) const noexcept
        {
            if (value)
            {
                *(p.get_port_address()) |= mask;
            }
            else
            {
                *(p.get_port_address()) &= ~mask;
            }
        }

        inline operator bool() const noexcept
        {
            return read();
        }
        inline bool operator=(bool value) const noexcept
        {
            write(value);
            return value;
        }
    };
} // namespace gb7

#endif // PORT_H
