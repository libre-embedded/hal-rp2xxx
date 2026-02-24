/* toolchain */
#include <iostream>

/* internal */
#include "generated/structs/uart.h"

int main(void)
{
    using namespace RP2XXX;
    std::cout << UART0_2040 << std::endl;
    std::cout << UART1_2040 << std::endl;
    std::cout << UART0_2350 << std::endl;
    std::cout << UART1_2350 << std::endl;

    return 0;
}
