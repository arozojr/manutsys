// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mqueue.h>
#include <pthread.h>
#include <fcntl.h>      // O_* flags
#include <sys/stat.h>   // mode constants
#include <time.h>

#include "manutencao.h"

// ---------- VARIÁVEIS GLOBAIS DO SERVIDOR ----------

// Buffer interno (produtor-consumidor)
Demanda buffer[BUFFER_SIZE];
int in_idx = 0;
int out_idx = 0;
int count = 0;

pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_has_item = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_has_space = PTHREAD_COND_INITIALIZER;

// ---------- FUNÇÃO UTILITÁRIA LOCAL ----------

static void erro(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

// ---------- THREAD DO TÉCNICO ----------

void *tecnico_thread(void *arg) {
    int id_tecnico = (int)(long)arg;

    while (1) {
        // Retira demanda do buffer interno
        pthread_mutex_lock(&buffer_mutex);

        while (count == 0) {
            // Espera até existir alguma demanda
            pthread_cond_wait(&cond_has_item, &buffer_mutex);
        }

        Demanda d = buffer[out_idx];
        out_idx = (out_idx + 1) % BUFFER_SIZE;
        count--;

        pthread_cond_signal(&cond_has_space);
        pthread_mutex_unlock(&buffer_mutex);

        // "Atende" a demanda
        printf("[TECNICO %d] Atendendo OS %d (maq=%d, prioridade=%d): %s\n",
               id_tecnico, d.id, d.maquina, d.prioridade, d.descricao);

        // Simula tempo de manutenção
        int tempo = 1 + rand() % 3;
        sleep(tempo);

        printf("[TECNICO %d] Concluiu OS %d em %d segundos.\n",
               id_tecnico, d.id, tempo);
    }

    return NULL;
}

// ---------- THREAD DISPATCHER (RECEBE DA MQ E COLOCA NO BUFFER) ----------

void *dispatcher_thread(void *arg) {
    mqd_t mq = *(mqd_t *)arg;
    Demanda d;

    while (1) {
        // Recebe da fila POSIX (bloqueante)
        unsigned int prioridade_mq;
        ssize_t n = mq_receive(mq, (char *)&d, sizeof(Demanda), &prioridade_mq);
        if (n < 0) {
            perror("mq_receive");
            continue;
        }

        printf("[DISPATCHER] Recebeu OS %d da máquina %d (prioridade=%u) msg=\"%s\"\n",
               d.id, d.maquina, prioridade_mq, d.descricao);

        // Insere no buffer interno (produtor-consumidor)
        pthread_mutex_lock(&buffer_mutex);

        while (count == BUFFER_SIZE) {
            // Buffer cheio: espera vaga
            pthread_cond_wait(&cond_has_space, &buffer_mutex);
        }

        buffer[in_idx] = d;
        in_idx = (in_idx + 1) % BUFFER_SIZE;
        count++;

        pthread_cond_signal(&cond_has_item);
        pthread_mutex_unlock(&buffer_mutex);
    }

    return NULL;
}

// ---------- MODO SERVIDOR ----------

void executar_servidor(void) {
    printf("== SERVIDOR DE MANUTENCAO INICIADO ==\n");

    // Configura atributos da fila de mensagens
    struct mq_attr attr;
    attr.mq_flags = 0; // bloqueante
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(Demanda);
    attr.mq_curmsgs = 0;

    // Cria (ou abre) a fila de mensagens
    mqd_t mq = mq_open(MQ_NAME, O_CREAT | O_RDONLY, 0660, &attr);
    if (mq == (mqd_t)-1) {
        erro("mq_open (servidor)");
    }

    printf("Fila POSIX '%s' criada/aberta.\n", MQ_NAME);

    // Cria thread dispatcher
    pthread_t dispatcher;
    if (pthread_create(&dispatcher, NULL, dispatcher_thread, &mq) != 0) {
        erro("pthread_create dispatcher");
    }

    // Cria threads dos técnicos
    pthread_t tecnicos[MAX_TECNICOS];
    for (int i = 0; i < MAX_TECNICOS; i++) {
        if (pthread_create(&tecnicos[i], NULL, tecnico_thread, (void *)(long)i) != 0) {
            erro("pthread_create tecnico");
        }
    }

    // Servidor simples: roda indefinidamente
    pthread_join(dispatcher, NULL);
    for (int i = 0; i < MAX_TECNICOS; i++) {
        pthread_join(tecnicos[i], NULL);
    }

    mq_close(mq);
    mq_unlink(MQ_NAME);
}