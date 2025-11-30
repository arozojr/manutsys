# 🛠️ Sistema de Manutenção com Fila POSIX, Threads e Buffer Produtor–Consumidor

Este projeto implementa um sistema de manutenção onde diferentes **máquinas (clientes)** enviam Ordens de Serviço (OS) para um **servidor**, que distribui essas OS entre **técnicos**, cada um executado como uma thread.

O sistema utiliza:

- Comunicação entre processos com **Filas POSIX** (`mq_*`)
- **Threads POSIX** (`pthread`)
- **Buffer circular** com sincronização (mutex + variáveis de condição)
- Arquitetura **Cliente → Servidor** com paralelismo

---

## 📁 Estrutura de Pastas

A estrutura do projeto segue um layout padrão para C:

```text
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
├── build/              # arquivos .o gerados na compilação
│
├── bin/                # executáveis finais
│   └── manutencao
│
├── Makefile            # regras de compilação
│
└── README.md           # documentação
```

---

## 🚀 Funcionalidades do Sistema

### ✔ Comunicação Cliente → Servidor via Fila POSIX  
Cada cliente envia uma struct `Demanda` para a fila `/fila_manutencao`.

### ✔ Dispatcher interno  
O servidor recebe mensagens da fila POSIX de forma **bloqueante** e repassa as OS para o buffer interno.

### ✔ Técnicos concorrentes  
Três threads de técnicos retiram OS do buffer circular e processam paralelamente.

### ✔ Controle de concorrência completo  
Implementação do clássico problema **Produtor–Consumidor**, usando:

- `pthread_mutex_t`
- `pthread_cond_t`
- Buffer circular

---

## 📦 Estrutura da OS

```c
typedef struct {
    int id;
    int maquina;
    int prioridade;
    char descricao[64];
} Demanda;
```

---

## 🧰 Compilação

O projeto possui um **Makefile** configurado para compilar tudo automaticamente.

Na raiz do projeto:

```bash
make
```

O executável final ficará em:

```
bin/manutencao
```

### Limpar arquivos gerados

```bash
make clean
```

---

## ▶ Como Executar

### 1️⃣ Iniciar o servidor

```bash
./bin/manutencao servidor
```

### 2️⃣ Iniciar um cliente

```bash
./bin/manutencao cliente <id_maquina> <num_os>
```

Exemplo:

```bash
./bin/manutencao cliente 1 5
```

### Vários clientes simultâneos

```bash
./bin/manutencao cliente 2 3
./bin/manutencao cliente 3 4
```

---

## 📡 Fluxo do Sistema

1. O **cliente** gera e envia OS para a fila POSIX.
2. O **dispatcher** do servidor lê a mensagem da fila.
3. O dispatcher insere a OS no **buffer circular interno**.
4. As threads de **técnicos** retiram OS do buffer e processam.
5. O fluxo continua indefinidamente.

---

## 📘 Conceitos Importantes (para estudo)

- **IPC (Inter-process Communication)** com Fila POSIX  
- **Produtor–Consumidor**  
- Threads POSIX (`pthread_create`, `pthread_join`)  
- Sincronização:
  - Mutex
  - Condições (`pthread_cond_wait`, `pthread_cond_signal`)
- Filas de mensagens com prioridade (`mq_send`, `mq_receive`)
- Buffer circular

---

## 📄 Licença

Uso livre para fins acadêmicos, pessoais e educacionais.
