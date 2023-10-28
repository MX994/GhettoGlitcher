#include "serial_trigger.hpp"
#include "hardware_map.hpp"

SerialTrigger::SerialTrigger() {
    // Initialize all variables.
    this->m_MatchBuffer = nullptr;
    this->m_Pattern = nullptr;
    this->m_PatternLength = 0;
    this->m_Baud = 0;
    this->m_DataBits = 0;
    this->m_StopBits = 0;
    this->m_Parity = 0;
    this->m_Armed = false;

    // Initialize default UART connection.
    uart_init(uart1, 9600);
    // gpio_set_function(UART_TX, GPIO_FUNC_UART);
    // gpio_set_function(UART_RX, GPIO_FUNC_UART);
    uart_set_hw_flow(uart1, false, false);
    uart_set_format(uart1, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(uart1, true);
}

void SerialTrigger::SetPattern(uint8_t *pattern, uint32_t size) {
    // If buffer was used before, wipe and free.
    if (this->m_MatchBuffer) {
        memset(this->m_MatchBuffer, 0, this->m_PatternLength);
        free(this->m_MatchBuffer);
        this->m_PatternLength = 0;
    }

    // Setup pattern & matching buffers.
    this->m_PatternLength = size;

    this->m_MatchBuffer = (uint8_t *)malloc(this->m_PatternLength);
    memset(this->m_MatchBuffer, 0, this->m_PatternLength);

    this->m_Pattern = (uint8_t *)malloc(this->m_PatternLength);
    memset(this->m_Pattern, 0, this->m_PatternLength);
}

void SerialTrigger::SetBaudRate(uint32_t baud) { 
    uart_set_baudrate(uart1, baud);
}

void SerialTrigger::SetFormat(uint32_t data_bits, uint32_t stop_bits, uart_parity_t parity) {
    uart_set_format(uart1, data_bits, stop_bits, parity);
}

void SerialTrigger::Arm() {
    if (!this->m_MatchBuffer) {
        return;
    }
    printf("Waiting for pattern!");
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
    this->m_Armed = true;
}

void SerialTrigger::Disarm() {
    printf("Stopped waiting for pattern.");
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
    this->m_Armed = false;
}

bool SerialTrigger::IsArmed() {
    return m_Armed;
}

// UART ISR.
void SerialTrigger::Test() {
    // Only parse if data && is armed.
    if (this->m_Armed) {
        // Shift all elements left by 1 (discarding buf[0])
        for (int i = 0; i < this->m_PatternLength - 1; ++i) {
            this->m_MatchBuffer[i] = this->m_MatchBuffer[i + 1];
        }
        printf((char *)this->m_MatchBuffer);
        // Add new element to buffer.
        uart_read_blocking(uart1, &this->m_MatchBuffer[this->m_PatternLength - 1], sizeof(char));

        // Check if it matches pattern.
        if (!memcmp(this->m_MatchBuffer, this->m_Pattern, this->m_PatternLength)) {
            // Time to trigger.
            gpio_put(EMP_TRIGGER_PIN, true);
            sleep_us(200);
            gpio_put(EMP_TRIGGER_PIN, false);
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);

            // Disable this to prevent multiple reports.
            this->Disarm();
        }
    }
}
