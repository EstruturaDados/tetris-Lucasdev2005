#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// ============================================================================
//        ESTRUTURA PRINCIPAL: PEÇA DE TETRIS
// ============================================================================
typedef struct {
    char tipo;
    int id;
} Peca;

// ============================================================================
//        FILA CIRCULAR DE PEÇAS FUTURAS
// ============================================================================
typedef struct {
    Peca itens[TAM_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

// ============================================================================
//        PILHA DE RESERVA DE PEÇAS
// ============================================================================
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

// ============================================================================
//        VARIÁVEIS GLOBAIS AUXILIARES
// ============================================================================
int contadorID = 0; // Gera IDs únicos para cada peça

// ============================================================================
//        FUNÇÕES DE GERAÇÃO E EXIBIÇÃO
// ============================================================================
char gerarTipoAleatorio() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    return tipos[rand() % 4];
}

Peca gerarPeca() {
    Peca nova;
    nova.tipo = gerarTipoAleatorio();
    nova.id = contadorID++;
    return nova;
}

void mostrarFila(Fila *fila) {
    printf("Fila de peças\t");
    if (fila->quantidade == 0) {
        printf("(vazia)");
    } else {
        int i;
        for (i = 0; i < fila->quantidade; i++) {
            int indice = (fila->frente + i) % TAM_FILA;
            printf("[ %c %d ] ", fila->itens[indice].tipo, fila->itens[indice].id);
        }
    }
    printf("\n");
}

void mostrarPilha(Pilha *pilha) {
    printf("Pilha de reserva\t(Topo -> Base): ");
    if (pilha->topo == -1) {
        printf("(vazia)");
    } else {
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[ %c %d ] ", pilha->itens[i].tipo, pilha->itens[i].id);
        }
    }
    printf("\n");
}

// ============================================================================
//        FUNÇÕES DA FILA CIRCULAR
// ============================================================================
void inicializarFila(Fila *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->quantidade = 0;
}

int filaCheia(Fila *fila) {
    return fila->quantidade == TAM_FILA;
}

int filaVazia(Fila *fila) {
    return fila->quantidade == 0;
}

void enqueue(Fila *fila, Peca peca) {
    if (filaCheia(fila)) return;
    fila->tras = (fila->tras + 1) % TAM_FILA;
    fila->itens[fila->tras] = peca;
    fila->quantidade++;
}

Peca dequeue(Fila *fila) {
    Peca removida = {'-', -1};
    if (filaVazia(fila)) {
        printf("⚠️ Fila vazia! Não há peça para jogar.\n");
        return removida;
    }
    removida = fila->itens[fila->frente];
    fila->frente = (fila->frente + 1) % TAM_FILA;
    fila->quantidade--;
    return removida;
}

// ============================================================================
//        FUNÇÕES DA PILHA
// ============================================================================
void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

int pilhaCheia(Pilha *pilha) {
    return pilha->topo == TAM_PILHA - 1;
}

int pilhaVazia(Pilha *pilha) {
    return pilha->topo == -1;
}

void push(Pilha *pilha, Peca peca) {
    if (pilhaCheia(pilha)) {
        printf("⚠️ Pilha cheia! Não é possível reservar mais peças.\n");
        return;
    }
    pilha->itens[++pilha->topo] = peca;
}

Peca pop(Pilha *pilha) {
    Peca removida = {'-', -1};
    if (pilhaVazia(pilha)) {
        printf("⚠️ Pilha vazia! Nenhuma peça reservada para usar.\n");
        return removida;
    }
    removida = pilha->itens[pilha->topo--];
    return removida;
}

// ============================================================================
//        OPERAÇÕES AVANÇADAS (NÍVEL MESTRE)
// ============================================================================
void trocarFrenteComTopo(Fila *fila, Pilha *pilha) {
    if (filaVazia(fila) || pilhaVazia(pilha)) {
        printf("⚠️ Não é possível trocar: fila ou pilha está vazia.\n");
        return;
    }
    int indiceFrente = fila->frente;
    Peca temp = fila->itens[indiceFrente];
    fila->itens[indiceFrente] = pilha->itens[pilha->topo];
    pilha->itens[pilha->topo] = temp;
    printf("✅ Peça da frente da fila trocada com o topo da pilha!\n");
}

void trocarTres(Fila *fila, Pilha *pilha) {
    if (fila->quantidade < 3 || pilha->topo != 2) {
        printf("⚠️ Para esta troca, a pilha deve ter 3 peças e a fila ao menos 3.\n");
        return;
    }
    for (int i = 0; i < 3; i++) {
        int indiceFila = (fila->frente + i) % TAM_FILA;
        Peca temp = fila->itens[indiceFila];
        fila->itens[indiceFila] = pilha->itens[pilha->topo - i];
        pilha->itens[pilha->topo - i] = temp;
    }
    printf("✅ Troca das 3 primeiras peças da fila com as 3 da pilha realizada!\n");
}

// ============================================================================
//        EXIBIÇÃO DO ESTADO GERAL
// ============================================================================
void mostrarEstado(Fila *fila, Pilha *pilha) {
    printf("\n----------------------------------------------\n");
    printf("Estado atual:\n\n");
    mostrarFila(fila);
    mostrarPilha(pilha);
    printf("----------------------------------------------\n");
}

// ============================================================================
//        FUNÇÃO PRINCIPAL
// ============================================================================
int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicialmente com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    int opcao;
    do {
        mostrarEstado(&fila, &pilha);
        printf("\nOpções de Ação:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("4 - Trocar peça da frente com topo da pilha\n");
        printf("5 - Trocar 3 primeiras da fila com as 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        printf("\n");

        switch (opcao) {
            case 1: {
                // Jogar peça
                Peca jogada = dequeue(&fila);
                if (jogada.id != -1)
                    printf("🎮 Peça jogada: [%c %d]\n", jogada.tipo, jogada.id);
                enqueue(&fila, gerarPeca());
                break;
            }
            case 2: {
                // Reservar peça
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca reservada = dequeue(&fila);
                    push(&pilha, reservada);
                    printf("📦 Peça reservada: [%c %d]\n", reservada.tipo, reservada.id);
                    enqueue(&fila, gerarPeca());
                } else {
                    printf("⚠️ Não foi possível reservar a peça.\n");
                }
                break;
            }
            case 3: {
                // Usar peça da reserva
                Peca usada = pop(&pilha);
                if (usada.id != -1)
                    printf("🧱 Peça usada da reserva: [%c %d]\n", usada.tipo, usada.id);
                break;
            }
            case 4:
                trocarFrenteComTopo(&fila, &pilha);
                break;
            case 5:
                trocarTres(&fila, &pilha);
                break;
            case 0:
                printf("👋 Encerrando o jogo...\n");
                break;
            default:
                printf("⚠️ Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
