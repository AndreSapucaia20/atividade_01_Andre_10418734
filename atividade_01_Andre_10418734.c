#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int codigo;
    char *nome;
    float preco;
    int quantidade;
} Produto;

void adicionar_produto(Produto **lista, int *qtd, int *prox_id);
void listar_produtos(Produto *lista, int qtd);
Produto* buscar_produto(Produto *lista, int qtd, int codigo);
void atualizar_estoque(Produto *lista, int qtd);
void remover_produto(Produto **lista, int *qtd);
void liberar_memoria(Produto **lista, int *qtd);
void limpar_buffer();

int main() {
    Produto *lista_produtos = NULL;
    int quantidade_atual = 0;
    int proximo_id = 1;
    int opcao;

    do {
        printf("\n========================================\n");
        printf("    SISTEMA DE CADASTRO DE PRODUTOS\n");
        printf("========================================\n");
        printf("1. Adicionar produto\n");
        printf("2. Listar produtos\n");
        printf("3. Buscar produto\n");
        printf("4. Atualizar estoque\n");
        printf("5. Remover produto\n");
        printf("6. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1:
                adicionar_produto(&lista_produtos, &quantidade_atual, &proximo_id);
                break;
            case 2:
                listar_produtos(lista_produtos, quantidade_atual);
                break;
            case 3:
                {
                    int cod;
                    printf("Digite o codigo para buscar: ");
                    scanf("%d", &cod);
                    Produto *p = buscar_produto(lista_produtos, quantidade_atual, cod);
                    if (p != NULL) {
                        printf("\nProduto Encontrado: %s | R$ %.2f | Qtd: %d\n", p->nome, p->preco, p->quantidade);
                    } else {
                        printf("\nProduto nao encontrado.\n");
                    }
                }
                break;
            case 4:
                atualizar_estoque(lista_produtos, quantidade_atual);
                break;
            case 5:
                remover_produto(&lista_produtos, &quantidade_atual);
                break;
            case 6:
                liberar_memoria(&lista_produtos, &quantidade_atual);
                break;
            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 6);

    return 0;
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 1. Adicionar Produto
void adicionar_produto(Produto **lista, int *qtd, int *prox_id) {
    char buffer_nome[100];
    float preco;
    int quantidade;

    printf("\n--- Adicionar Produto ---\n");
    printf("Nome: ");
    scanf(" %[^\n]", buffer_nome);
    printf("Preco: ");
    scanf("%f", &preco);
    printf("Quantidade: ");
    scanf("%d", &quantidade);

    Produto *temp = realloc(*lista, (*qtd + 1) * sizeof(Produto));
    
    if (temp == NULL) {
        printf("Erro: Memoria insuficiente!\n");
        return;
    }
    *lista = temp;

    (*lista)[*qtd].codigo = *prox_id;
    (*lista)[*qtd].preco = preco;
    (*lista)[*qtd].quantidade = quantidade;

    (*lista)[*qtd].nome = (char*) malloc((strlen(buffer_nome) + 1) * sizeof(char));
    if ((*lista)[*qtd].nome == NULL) {
        printf("Erro ao alocar memoria para o nome!\n");
        return; 
    }
    strcpy((*lista)[*qtd].nome, buffer_nome);

    printf("Produto adicionado com codigo %d!\n", *prox_id);

    (*prox_id)++;
    (*qtd)++;
}

// 2. Listar Todos os Produtos
void listar_produtos(Produto *lista, int qtd) {
    if (qtd == 0) {
        printf("\nNenhum produto cadastrado.\n");
        return;
    }

    printf("\n--- Lista de Produtos ---\n");
    printf("+--------+----------------------+----------+------+---------------+\n");
    printf("| Codigo | Nome                 | Preco    | Qtd  | Valor Estoque |\n");
    printf("+--------+----------------------+----------+------+---------------+\n");

    float total_estoque = 0;

    for (int i = 0; i < qtd; i++) {
        float valor_item = lista[i].preco * lista[i].quantidade;
        total_estoque += valor_item;
        
        printf("| %6d | %-20s | %8.2f | %4d | %13.2f |\n", 
               lista[i].codigo, 
               lista[i].nome, 
               lista[i].preco, 
               lista[i].quantidade, 
               valor_item);
    }
    printf("+--------+----------------------+----------+------+---------------+\n");
    printf("Valor total do estoque: R$ %.2f\n", total_estoque);
}

// 3. Buscar Produto por Código (Retorna ponteiro)
Produto* buscar_produto(Produto *lista, int qtd, int codigo) {
    for (int i = 0; i < qtd; i++) {
        if (lista[i].codigo == codigo) {
            return &lista[i];
        }
    }
    return NULL;
}

// 4. Atualizar Estoque
void atualizar_estoque(Produto *lista, int qtd) {
    int cod, nova_qtd;
    
    printf("\n--- Atualizar Estoque ---\n");
    printf("Codigo do produto: ");
    scanf("%d", &cod);

    Produto *p = buscar_produto(lista, qtd, cod);

    if (p != NULL) {
        printf("Produto: %s | Estoque atual: %d\n", p->nome, p->quantidade);
        printf("Nova quantidade: ");
        scanf("%d", &nova_qtd);
        
        p->quantidade = nova_qtd; 
        printf("Estoque atualizado com sucesso!\n");
    } else {
        printf("Produto nao encontrado.\n");
    }
}

// 5. Remover Produto
void remover_produto(Produto **lista, int *qtd) {
    int cod;
    printf("\n--- Remover Produto ---\n");
    printf("Codigo do produto: ");
    scanf("%d", &cod);

    int indice = -1;
    for (int i = 0; i < *qtd; i++) {
        if ((*lista)[i].codigo == cod) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Produto nao encontrado.\n");
        return;
    }

    printf("Produto \"%s\" removido com sucesso!\n", (*lista)[indice].nome);
    free((*lista)[indice].nome);

    for (int i = indice; i < *qtd - 1; i++) {
        (*lista)[i] = (*lista)[i + 1];
    }

    (*qtd)--;
    
    if (*qtd == 0) {
        free(*lista);
        *lista = NULL;
    } else {
        Produto *temp = realloc(*lista, (*qtd) * sizeof(Produto));
        if (temp != NULL) {
            *lista = temp;
        }
    }
}

// 6. Sair e Liberar Memória
void liberar_memoria(Produto **lista, int *qtd) {
    printf("\nLiberando memoria...\n");
    
    if (*lista != NULL) {
        for (int i = 0; i < *qtd; i++) {
            free((*lista)[i].nome);
        }
        free(*lista); 
        *lista = NULL;
    }
    
    printf("Memoria liberada. Programa encerrado.\n");
}
