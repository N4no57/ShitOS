#include "../../../intf/commands.h"
#include "../../../intf/stdio.h"
#include "../../../intf/string.h"

command_entry_t commands[] = {
    { "help", cmd_help },
    { "exit", cmd_exit },
    { "clear", cmd_clear },
    { "echo", cmd_echo },
    { "halt", cmd_halt },
    { "reboot", cmd_reboot },
    { "exit", cmd_exit }
};

int command_count = sizeof(commands) / sizeof(commands[0]);

int run_command(int argc, char **argv) {
    char name[strlen(argv[0])];
    strcpy(name, argv[0]);
    for (int i = 0; i < command_count; i++) {
        if (strcmp(name, commands[i].name) == 0) {
            return commands[i].func(argc, argv);
        }
    }

    if (argv[0] == 0x0) {
        printf("Command ' ' not found\n");
        return -1;
    }

    printf("Command '%s' not found\n", name);
    return -1;
}