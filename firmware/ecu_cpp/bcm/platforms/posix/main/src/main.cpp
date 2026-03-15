// Taktflow Systems - BCM POSIX entry point

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

extern void terminal_cleanup(void);
extern void main_thread_setup(void);
extern void app_main();

extern "C"
{
void putchar_(char character) { putchar(character); }
}

static void intHandler(int /* sig */)
{
    terminal_cleanup();
    _exit(0);
}

int main()
{
    signal(SIGINT, intHandler);
    signal(SIGTERM, intHandler);
    main_thread_setup();
    app_main();
    return 1; // never reached
}
