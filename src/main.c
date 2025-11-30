// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "manutencao.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr,
                "Uso:\n"
                "  %s servidor\n"
                "  %s cliente <id_maquina> <num_os>\n",
                argv[0], argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "servidor") == 0) {
        executar_servidor();
    } else if (strcmp(argv[1], "cliente") == 0) {
        if (argc < 4) {
            fprintf(stderr, "Uso: %s cliente <id_maquina> <num_os>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        int maquina = atoi(argv[2]);
        int num_os = atoi(argv[3]);
        executar_cliente(maquina, num_os);
    } else {
        fprintf(stderr, "Modo desconhecido: %s (use 'servidor' ou 'cliente')\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    return 0;
}