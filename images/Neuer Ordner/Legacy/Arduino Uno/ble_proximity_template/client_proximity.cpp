#include <stdint.h>
#include "lib_aci.h"
#include "services.h"
#include "client_proximity.h"


void client_proximity_pipes_updated_evt_rcvd(uint8_t pipe_num, uint8_t *buffer)
{
  switch (pipe_num)
  {
    case PIPE_PROXIMITY_MONITORING_CLIENT_PROXIMITY_RX :
      client_proximity_hook(buffer[0],buffer[1]);
      break;
  }
}
