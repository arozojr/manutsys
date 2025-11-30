// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>      // O_* flags
#include <sys/stat.h>   // mode constants
#include <time.h>

#include "manutencao.h"

// ---------- FUNÇÃO UTILITÁRIA LOCAL ----------

static void erro(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

// ---------- MODO CLIENTE (MÁQUINA) ----------

void executar_cliente(int maquina, int num_os) {
    printf("== CLIENTE (MAQUINA %d) GERANDO OS ==\n", maquina);

    mqd_t mq = mq_open(MQ_NAME, O_WRONLY);
    if (mq == (mqd_t)-1) {
        erro("mq_open (cliente)");
    }

    srand(time(NULL) ^ getpid());

    for (int i = 0; i < num_os; i++) {
        Demanda d;
        d.id = (maquina * 1000) + i;  // Ex: máquina 2 -> OS 2000, 2001...
        d.maquina = maquina;
        d.prioridade = 1 + rand() % 5; // 1 a 5
        snprintf(d.descricao, sizeof(d.descricao),
                 "Problema simulado %d da maquina %d", i, maquina);

        unsigned int prioridade_mq = d.prioridade;

        if (mq_send(mq, (const char *)&d, sizeof(Demanda), prioridade_mq) < 0) {
            perror("mq_send");
        } else {
            printf("[MAQ %d] Enviou OS %d (prioridade=%d)\n",
                   maquina, d.id, d.prioridade);
        }

        sleep(1 + rand() % 2); // espera um pouco antes da próxima OS
    }

    mq_close(mq);
    printf("== CLIENTE (MAQUINA %d) FINALIZADO ==\n", maquina);
}