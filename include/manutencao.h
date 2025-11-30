// manutencao.h
#ifndef MANUTENCAO_H
#define MANUTENCAO_H

#include <mqueue.h>

#define MQ_NAME "/fila_manutencao"

#define MAX_TECNICOS 3
#define BUFFER_SIZE 10

typedef struct {
    int id;
    int maquina;
    int prioridade;
    char descricao[64];
} Demanda;

// Protótipos
void executar_servidor(void);
void executar_cliente(int maquina, int num_os);

#endif // MANUTENCAO_H