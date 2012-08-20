#include "shell_header.h"
#include "spi.h"
#include "discovery_accel.h"
#include "math.h"

typedef struct accel_data {
    int8_t this;
    int8_t x;
    int8_t stuff;
    int8_t y;
    int8_t sucks;
    int8_t z;
} accel_data;

void accel(int argc, char **argv) {
    if (argc != 1) {
        printf("Usage: %s\r\n", argv[0]);
        return;
    }
    rd_t accelrd = open_discovery_accel();
    accel_data *data = malloc(sizeof(accel_data));
    printf("q to quit, any other key to get data.\r\nunits in g's\r\n");
    while(1) {
        if(getc() == 'q') {
            free(data);
            close(accelrd);
            return;
        }
        else {
            read(accelrd, (char *)data, 6);
            printf("X: %f Y: %f Z: %f Roll: %f\r\n", data->x*DISCOVERY_ACCEL_SENSITIVITY, data->y*DISCOVERY_ACCEL_SENSITIVITY, data->z*DISCOVERY_ACCEL_SENSITIVITY, atan2(data->z*DISCOVERY_ACCEL_SENSITIVITY, data->y*DISCOVERY_ACCEL_SENSITIVITY)*RAD_TO_DEG);
        }
    }

    free(data);
    close(accelrd);
}
