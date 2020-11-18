#include <stdint.h>
#include "lib_aci.h"
#include "services.h"
#include "client_time.h"


void client_time_pipes_updated_evt_rcvd(uint8_t pipe_num, uint8_t *buffer)
{
  switch (pipe_num)
  {
    case PIPE_TIME_MONITORING_CLIENT_TIME_RX :
      client_time_hook(buffer[0],buffer[1],buffer[2]);
      break;
  }
}
