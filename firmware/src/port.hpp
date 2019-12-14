#ifndef PORT_H
#define PORT_H

#include <avr/io.h>

namespace gb7
{
    enum class port_type
    {
        PortB, PortC, PortD,
    };
    using pin_number = uint16_t;


#if defined __cpp_concepts && __cpp_concepts >= 201907
    /*
     * concepts
     */
    template<typename T>
    concept PinReadable = requires(T& p)
    {
        { p.read() } -> bool;
    };
    
    template<typename T>
    concept PinWritable = requires(T& p)
    {
        p.write;
    };

    template<typename T>
    concept PortReadable = requires(T& p)
    {
        { p.read() } -> uint8_t;
        p.get_readable_pin;
    };
    
    template<typename T>
    concept PortWritable = requires(T& p)
    {
        p.write;
        p.get_writable_pin;
    };
#endif


    /*
     * definition
     */
    template<port_type P>
    class port_address_converter
    {
    public:
        port_address_converter() = delete;
        ~port_address_converter() = delete;

        [[nodiscard]] constexpr static volatile uint8_t* get_ddr_address() noexcept
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
        [[nodiscard]] constexpr static volatile uint8_t* get_port_address() noexcept
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
        [[nodiscard]] constexpr static volatile uint8_t* get_pin_address() noexcept
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


    template<port_type P, pin_number N>
    class pin_writable
    {
    private:
        inline static constexpr uint8_t mask = (1 << N);

    public:
        inline bool read() const noexcept
        {
            return (*(port_address_converter<P>::get_port_address()) & mask) != 0;
        }

        inline void write(bool value) const noexcept
        {
            if (value)
            {
                *(port_address_converter<P>::get_port_address()) |= mask;
            }
            else
            {
                *(port_address_converter<P>::get_port_address()) &= ~mask;
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

    template<port_type P, pin_number N>
    class pin_readable
    {
    private:
        inline static constexpr uint8_t mask = (1 << N);

    public:
        inline bool read() const noexcept
        {
            return (*(port_address_converter<P>::get_pin_address()) & mask) != 0;
        }

        inline operator bool() const noexcept
        {
            return read();
        }
    };


    template<port_type P>
    class port_writable
    {
    public:
        port_writable(uint8_t reversed = 0x00) noexcept
        {
            *(port_address_converter<P>::get_ddr_address()) = 0xff;
            if (reversed != 0x00)
                *(port_address_converter<P>::get_pin_address()) = reversed;
        }

        inline uint8_t read() const noexcept
        {
            return *(port_address_converter<P>::get_port_address());
        }
        inline void write(uint8_t value) const noexcept
        {
            *(port_address_converter<P>::get_port_address()) = value;
        }

        inline operator uint8_t() const noexcept
        {
            return read();
        }
        inline uint8_t operator=(uint8_t v) const noexcept
        {
            write(v);
            return v;
        }

        template<pin_number N>
        inline auto get_writable_pin() noexcept
        {
            constexpr pin_number max_n = (P == port_type::PortC ? 6 : 7);
            static_assert(N <= max_n, "Invalid pin number");
            return pin_writable<P, N> {};
        }
    };

    template<port_type P>
    class port_readable
    {
    public:
        port_readable(uint8_t pull_up = 0xff) noexcept
        {
            *(port_address_converter<P>::get_ddr_address()) = 0x00;
            if (pull_up != 0x00)
                *(port_address_converter<P>::get_port_address()) = pull_up;
        }

        inline uint8_t read() const noexcept
        {
             return *(port_address_converter<P>::get_pin_address());
        }

        inline operator uint8_t() const noexcept
        {
            return read();
        }

        template<pin_number N>
        inline auto get_readable_pin() noexcept
        {
            constexpr pin_number max_n = (P == port_type::PortC ? 6 : 7);
            static_assert(N <= max_n, "Invalid pin number");
            return pin_readable<P, N> {};
        }
    };

    enum pin_io_config: uint8_t { readable = 0, writable = 1 };
    template<port_type P, const pin_io_config config[8]>
    class port_mixed
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
            *(port_address_converter<P>::get_ddr_address()) = mask;
        }

        inline uint8_t read() const noexcept
        {
            return *(port_address_converter<P>::get_port_address());
        }
        inline void write(uint8_t value) const noexcept
        {
            *(port_address_converter<P>::get_port_address()) = value;
        }

        inline operator uint8_t() const noexcept
        {
            return read();
        }
        inline uint8_t operator=(uint8_t v) const noexcept
        {
            write(v);
            return v;
        }

        template<pin_number N>
        inline auto get_readable_pin() noexcept
        {
            constexpr pin_number max_n = (P == port_type::PortC ? 6 : 7);
            static_assert(N <= max_n, "Invalid pin number");
            static_assert(config[N] == pin_io_config::readable, "Tried to read non-readable pin!");
            return pin_readable<P, N> {};
        }

        template<pin_number N>
        inline auto get_writable_pin() noexcept
        {
            constexpr pin_number max_n = (P == port_type::PortC ? 6 : 7);
            static_assert(N <= max_n, "Invalid pin number");
            static_assert(config[N] == pin_io_config::writable, "Tried to write to non-writable pin!");
            return pin_writable<P, N> {};
        }
    };

    template<port_type P>
    class port_dyamic
    {
    public:
        port_dyamic() noexcept
        {
            *(port_address_converter<P>::get_ddr_address()) = 0xff;
        }

        inline void set_direction(pin_io_config config[8]) noexcept
        {
            const uint8_t mask =
                (config[0] == pin_io_config::writable ? 1 : 0) << 0 |
                (config[1] == pin_io_config::writable ? 1 : 0) << 1 |
                (config[2] == pin_io_config::writable ? 1 : 0) << 2 |
                (config[3] == pin_io_config::writable ? 1 : 0) << 3 |
                (config[4] == pin_io_config::writable ? 1 : 0) << 4 |
                (config[5] == pin_io_config::writable ? 1 : 0) << 5 |
                (config[6] == pin_io_config::writable ? 1 : 0) << 6 |
                (config[7] == pin_io_config::writable ? 1 : 0) << 7;
            
            *(port_address_converter<P>::get_ddr_address()) = mask;
        }
        inline void set_direction(uint8_t mask) noexcept
        {
            *(port_address_converter<P>::get_ddr_address()) = mask;
        }

        inline uint8_t read() const noexcept
        {
            return *(port_address_converter<P>::get_port_address());
        }
        inline void write(uint8_t value) const noexcept
        {
            *(port_address_converter<P>::get_port_address()) = value;
        }

        inline operator uint8_t() const noexcept
        {
            return read();
        }
        inline uint8_t operator=(uint8_t v) const noexcept
        {
            write(v);
            return v;
        }

        template<pin_number N>
        inline auto get_readable_pin() noexcept
        {
            return pin_readable<P, N> {};
        }

        template<pin_number N>
        inline auto get_writable_pin() noexcept
        {
            return pin_writable<P, N> {};
        }
    };
} // namespace gb7

#endif // PORT_H
