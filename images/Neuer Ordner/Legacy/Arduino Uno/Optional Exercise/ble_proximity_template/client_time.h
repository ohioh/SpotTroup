#include <stdint.h>

/** @defgroup client_time
* @{
* @ingroup services
*  
* @brief time_monitoring service implementation.
* 
*/

#ifndef CLIENT_TIME__
#define CLIENT_TIME__

/** @brief Callback on client time.
 *  @details This function is called when a client time characteristic value is updated. 
 *  @param client_time_data : 3 octets; 1st contains hour value (HH), 2nd contains minute value (MM) and the 3rd the second value (SS)
 */
extern void client_time_hook(uint8_t hh,uint8_t mm,uint8_t ss);

/** @brief Function to handle reception of data for client_time.
 *  @details Call this function on the ACI_EVT_DATA_RECEIVED event. This function checks if the data received is for client time.
 *  If so it triggers the client time with the received level.
 *  @param pipe_num Pipe which has received data.
 *  @param buffer Pointer to the received data.
 */
void client_time_pipes_updated_evt_rcvd(uint8_t pipe_num, uint8_t *buffer);

#endif//CLIENT_TIME__
/** @} */

