#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
using namespace std;

// Estrutura do nó da árvore binária
struct No {
    string nomeCientifico;    // chave da árvore
    string nomePopular;
    int populacaoEstimada;
    string regiao;
    string statusConservacao;
    No* esq;
    No* dir;

    No(string nomeCient, string nomePop, int populacao, string reg, string status)
        : nomeCientifico(nomeCient), nomePopular(nomePop), populacaoEstimada(populacao),
        regiao(reg), statusConservacao(status), esq(nullptr), dir(nullptr) {
    }
};

// função pra inserir nó na árvore 
No* inserir(No* atual, const string& nomeCientifico, const string& nomePopular,
    int populacao, const string& regiao, const string& status) {
    if (atual == nullptr) {
        return new No(nomeCientifico, nomePopular, populacao, regiao, status);
    }
    if (nomeCientifico < atual->nomeCientifico) {
        atual->esq = inserir(atual->esq, nomeCientifico, nomePopular, populacao, regiao, status);
    }
    else if (nomeCientifico > atual->nomeCientifico) {
        atual->dir = inserir(atual->dir, nomeCientifico, nomePopular, populacao, regiao, status);
    }
    return atual;
}

// função pra buscar nó 
No* buscar(No* atual, const string& nomeCientifico) {
    if (atual == nullptr || nomeCientifico == atual->nomeCientifico) {
        return atual;
    }
    if (nomeCientifico < atual->nomeCientifico) {
        return buscar(atual->esq, nomeCientifico);
    }
    return buscar(atual->dir, nomeCientifico);
}

// função pra excluir nó 
No* excluir(No* atual, const string& nomeCientifico) {
    if (atual == nullptr) return nullptr;

    if (nomeCientifico < atual->nomeCientifico) {
        atual->esq = excluir(atual->esq, nomeCientifico);
    }
    else if (nomeCientifico > atual->nomeCientifico) {
        atual->dir = excluir(atual->dir, nomeCientifico);
    }
    else {
        if (atual->esq == nullptr && atual->dir == nullptr) {
            delete atual;
            return nullptr;
        }
        if (atual->esq == nullptr) {
            No* temp = atual->dir;
            delete atual;
            return temp;
        }
        if (atual->dir == nullptr) {
            No* temp = atual->esq;
            delete atual;
            return temp;
        }
        No* sucessor = atual->dir;
        while (sucessor->esq != nullptr) {
            sucessor = sucessor->esq;
        }
        atual->nomeCientifico = sucessor->nomeCientifico;
        atual->nomePopular = sucessor->nomePopular;
        atual->populacaoEstimada = sucessor->populacaoEstimada;
        atual->regiao = sucessor->regiao;
        atual->statusConservacao = sucessor->statusConservacao;
        atual->dir = excluir(atual->dir, sucessor->nomeCientifico);
    }
    return atual;
}

// função pra listar os nós em ordem
void inOrder(No* atual, FILE* out) {
    if (atual != nullptr) {
        inOrder(atual->esq, out);
        fprintf(out, "%s (%s) - Populacao: %d - Regiao: %s - Status: %s\n",
            atual->nomeCientifico.c_str(), atual->nomePopular.c_str(),
            atual->populacaoEstimada, atual->regiao.c_str(),
            atual->statusConservacao.c_str());
        inOrder(atual->dir, out);
    }
}

// função pra estatísticas
void calcularEstatisticas(No* atual, int& total, int& maior, int& menor, string& maiorNome,
    string& menorNome, map<string, int>& statusCount) {
    if (atual != nullptr) {
        total++;
        if (atual->populacaoEstimada > maior) {
            maior = atual->populacaoEstimada;
            maiorNome = atual->nomeCientifico;
        }
        if (atual->populacaoEstimada < menor) {
            menor = atual->populacaoEstimada;
            menorNome = atual->nomeCientifico;
        }
        statusCount[atual->statusConservacao]++;
        calcularEstatisticas(atual->esq, total, maior, menor, maiorNome, menorNome, statusCount);
        calcularEstatisticas(atual->dir, total, maior, menor, maiorNome, menorNome, statusCount);
    }
}

void imprimirEspacos(int nivel) {
    for (int i = 0; i < nivel; ++i) {
        printf("    ");
    }
}

void imprimirDiagrama(No* raiz, int nivel) {
    if (raiz == NULL) {
        imprimirEspacos(nivel);
        printf("(vazio)\n");
        return;
    }

    imprimirEspacos(nivel);
    printf("[%s]\n", raiz->nomeCientifico.c_str());

    imprimirEspacos(nivel);
    printf(" |- Esquerda:\n");
    imprimirDiagrama(raiz->esq, nivel + 1);

    imprimirEspacos(nivel);
    printf(" |- Direita:\n");
    imprimirDiagrama(raiz->dir, nivel + 1);
}

// menu principal 
void exibirMenu() {
    No* raiz = nullptr;
    int opcao;

    do {
        printf("\nMenu:\n");
        printf("1. Inserir nova especie\n");
        printf("2. Buscar especie\n");
        printf("3. Excluir especie\n");
        printf("4. Listar especies (In-order)\n");
        printf("5. Exportar catalogo ordenado\n");
        printf("6. Exibir estatisticas\n");
        printf("7. Sair\n");
        printf("Escolha uma opcao: ");
        scanf_s("%d", &opcao);
        cin.ignore(); // limpar espaço de digitação após o scanf

        switch (opcao) {
        case 1: {
            string nomeCient, nomePop, regiao, status;
            int populacao;
            printf("Nome cientifico: ");
            getline(cin, nomeCient);
            printf("Nome popular: ");
            getline(cin, nomePop);
            printf("Populacao estimada: ");
            scanf_s("%d", &populacao);
            cin.ignore(); // limpar espaço de digitação após o scanf
            printf("Regiao de ocorrencia: ");
            getline(cin, regiao);
            printf("Status de conservacao: ");
            getline(cin, status);

            raiz = inserir(raiz, nomeCient, nomePop, populacao, regiao, status);
            break;
        }
        case 2: {
            string nomeCient;
            printf("Nome cientifico para busca: ");
            getline(cin, nomeCient);

            No* resultado = buscar(raiz, nomeCient);
            if (resultado != nullptr) {
                printf("Especie encontrada: %s\n", resultado->nomeCientifico.c_str());
                printf("Nome popular: %s\n", resultado->nomePopular.c_str());
                printf("Populacao estimada: %d\n", resultado->populacaoEstimada);
                printf("Regiao: %s\n", resultado->regiao.c_str());
                printf("Status de conservacao: %s\n", resultado->statusConservacao.c_str());
            }
            else {
                printf("Especie nao encontrada.\n");
            }
            break;
        }
        case 3: {
            string nomeCient;
            printf("Nome cientifico para exclusao: ");
            getline(cin, nomeCient);

            raiz = excluir(raiz, nomeCient);
            printf("Especie removida com sucesso.\n");
            break;
        }
        case 4: {
            inOrder(raiz, stdout);
            break;
        }
        case 5: {
            FILE* arquivo;
            errno_t err = fopen_s(&arquivo, "catalogo.txt", "w");
            if (arquivo != nullptr) {
                inOrder(raiz, arquivo);
                fclose(arquivo);
                printf("Catalogo exportado para catalogo.txt\n");
            }
            else {
                printf("Erro ao criar o arquivo.\n");
            }
            break;
        }
        case 6: {
            int total = 0, maior = -1, menor = INT_MAX;
            string maiorNome, menorNome;
            map<string, int> statusCount;

            calcularEstatisticas(raiz, total, maior, menor, maiorNome, menorNome, statusCount);
            printf("Total de especies: %d\n", total);
            printf("Especie com maior populacao (%d): %s\n", maior, maiorNome.c_str());
            printf("Especie com menor populacao (%d): %s\n", menor, menorNome.c_str());

            for (const auto& pair : statusCount) {
                printf("%d especie(s) com status \"%s\"\n", pair.second, pair.first.c_str());
            }
            break;
        }
        case 7:
            printf("Ate logo!\n");
            break;
        default:
            printf("Opcao invalida! Escolha outra\n");
        }
    } while (opcao != 7);
}

int main() {
    exibirMenu();
    return 0;
}