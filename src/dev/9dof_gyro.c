#include "dev_header.h"
#include "spi.h"
#include "discovery_accel.h"

extern spi_dev spi1;

rd_t open_9dof_gyro(void) {
    9dof_gyro *gyro = kmalloc(sizeof(9dof_gyro));
    resource *new_r = kmalloc(sizeof(resource));
    /* We expect that spi1 was init'd in bootmain.c */
    accel->spi_port = &spi1;
    accel->spi_port->write(0x20, 0x47);
    accel->read_ctr = 0;
    new_r->env = gyro;
    new_r->writer = &discovery_accel_write;
    new_r->reader = &discovery_accel_read;
    new_r->closer = &discovery_accel_close;
    new_r->sem = kmalloc(sizeof(semaphore));
    init_semaphore(new_r->sem);
    add_resource(curr_task->task, new_r);
    return curr_task->task->top_rd - 1;
}

char 9dof_gyro_read(void *env) {
    discovery_accel *accel = (discovery_accel *)env;
    if(accel->read_ctr > 5)
        accel->read_ctr = 0;
    return (char)accel->spi_port->read(0x28 + accel->read_ctr++);
}

void 9dof_gyro_write(char d, void *env) {
    /* No real meaning to this yet */
}

void 9dof_gyro_close(void *env) {
    kfree(env);
}
