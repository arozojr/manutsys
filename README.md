🛠️ Sistema de Manutenção com Fila POSIX, Threads e Buffer Produtor–Consumidor
Este projeto implementa um sistema de manutenção onde diferentes máquinas (clientes) enviam Ordens de Serviço (OS) para um servidor, que por sua vez distribui essas OS para técnicos executarem em paralelo.
Ele combina:
IPC via Fila POSIX (mq_*)
Programação concorrente com Threads (pthread)
Buffer circular com sincronização (mutex + cond_var)

📁 Estrutura de Pastas
A organização do projeto segue um padrão simples e eficiente para C:
meu_projeto/
│
├── src/                # código-fonte (.c)
│   ├── server.c
│   ├── client.c
│   └── main.c
│
├── include/            # headers (.h)
│   └── manutencao.h
│
├── build/              # arquivos gerados na compilação (.o)
│
├── bin/                # executáveis finais
│   └── manutencao
│
├── Makefile            # regras de compilação
│
└── README.md           # documentação

🚀 Funcionalidades do Sistema
✔ Comunicação Cliente → Servidor via Fila POSIX
Cada cliente envia uma struct Demanda para a fila /fila_manutencao.

✔ Dispatcher interno
O servidor recebe mensagens da fila e coloca em um buffer circular seguro.

✔ Técnicos concorrentes
Três threads de técnicos retiram OS do buffer e processam em paralelo.

✔ Controle de concorrência completo
Implementação clássica do problema Produtor–Consumidor com:
Mutex
Variáveis de condição
Buffer circular

📦 Estrutura da OS
typedef struct {
    int id;
    int maquina;
    int prioridade;
    char descricao[64];
} Demanda;

🧰 Compilação
O projeto já inclui um Makefile.
Basta executar:
make

O executável será gerado em:
bin/manutencao

Para limpar os arquivos gerados:
make clean

▶ Como Executar
1. Inicie o servidor
./bin/manutencao servidor

2. Execute um cliente
./bin/manutencao cliente <id_maquina> <num_os>

Exemplo:
./bin/manutencao cliente 1 5

Você pode executar vários clientes simultaneamente:
./bin/manutencao cliente 2 3
./bin/manutencao cliente 3 4

📡 Fluxo de Funcionamento
O cliente envia uma OS para a fila POSIX.
O dispatcher do servidor lê da fila.
O dispatcher insere no buffer interno.
Técnicos (threads) consomem as OS do buffer.
O processo continua indefinidamente.

📘 Conceitos Importantes
Este projeto é excelente para estudar:
Fila POSIX (mq_open, mq_send, mq_receive)
Threads (pthread_create, pthread_join)

Sincronização:
Mutex
Variáveis de condição
Buffer circular
Comunicação entre processos (IPC)
Arquitetura Cliente/Servidor

📄 Licença
Uso livre para fins educacionais, acadêmicos e pessoais.
