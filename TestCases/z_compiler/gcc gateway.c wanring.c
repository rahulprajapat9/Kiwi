The following warning is there in line 88 of gateway.c, for check, I have put up cmd = 1.


src/gateway.c: In function ‘handle_communication’:
src/gateway.c:88:23: error: initialization makes integer from pointer without a cast [-Werror=int-conversion]
         uint8_t cmd = &(&backend_recv_data)[16];
                       ^
cc1: all warnings being treated as errors


