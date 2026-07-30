#include "../codexion.h"

long get_time_ms(void)
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);

    return ((current_time.tv_sec * 1000)
        +(current_time.tv_usec / 1000));
}


long get_timestamp(t_simulation *sim)
{
    long current_time;

    current_time = get_time_ms();
    return (current_time - sim->start_time);
}

void smart_sleep(long duration, t_simulation *sim)
{
    long start;

    start = get_timestamp(sim);
    while((get_timestamp(sim) - start) < duration)
    {
        if (sim->stop)
            return;
        usleep(1000);
    }
}