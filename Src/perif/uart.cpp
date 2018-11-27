#include "perif/uart.h"

namespace quad
{
UART::UART(USART_TypeDef *uart)
:m_uart(uart)
{
	m_subscriber = nullptr;
}
void UART::StartDataTransmit()
{
	LL_USART_EnableIT_TXE(m_uart);
}
void UART::SendData(uint8_t *data, uint32_t size)
{
	StartDataTransmit();
}
void UART::SendData(uint8_t *str)
{
	uint32_t len = 0;
	for (len = 0; str[len]; len++);
	SendData(str, len);
}
void UART::InterruptHandler()
{
	uint8_t ch;
	if (LL_USART_IsActiveFlag_TXE(m_uart))
	{
		if (m_outData.pop(ch))
			LL_USART_TransmitData8(m_uart, ch);
		else
			LL_USART_DisableIT_TXE(m_uart);
	}
	if (LL_USART_IsActiveFlag_RXNE(m_uart))
	{
		ch = LL_USART_ReceiveData8(m_uart);
		m_inData.push(ch);
	}
}
}
