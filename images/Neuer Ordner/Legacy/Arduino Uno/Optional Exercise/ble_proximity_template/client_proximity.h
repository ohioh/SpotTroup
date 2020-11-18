#include <stdint.h>

/** @defgroup client_proximity
* @{
* @ingroup services
*  
* @brief client_proximity service implementation.
* 
*/

#ifndef CLIENT_PROXIMITY__
#define CLIENT_PROXIMITY__

/** @brief Callback on client proximity.
 *  @details This function is called when an client proximity characteristic value is updated. 
 *  @param client_proximity_data : 2 octets; 1st contains proximity band indicator (0x00,0x01 or 0x02)
 *                                 and the 2nd contains the RSSI value
 */
extern void client_proximity_hook(uint8_t proximity_band, uint8_t rssi);

/** @brief Function to handle reception of data for client_proximity.
 *  @details Call this function on the ACI_EVT_DATA_RECEIVED event. This function checks if the data received is for client proximity.
 *  If so it triggers the client proximity with the received level.
 *  @param pipe_num Pipe which has received data.
 *  @param buffer Pointer to the received data.
 */
void client_proximity_pipes_updated_evt_rcvd(uint8_t pipe_num, uint8_t *buffer);

#endif//CLIENT_PROXIMITY__
/** @} */

