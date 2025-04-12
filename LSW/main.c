#include <stdio.h>
#include <stdlib.h>
#include "processo.h"

// Funcao para contar o número de linhas (registros) no csv, excluindo o cabeçalho


int contar_linhas(const char *nome_arquivo) {
    FILE *fp = fopen(nome_arquivo, "r");
    if (!fp) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }
    char linha[1024];
    int count = 0;

    // Lê e descarta o cabeçalho
    if (fgets(linha, sizeof(linha), fp) == NULL) {
        fclose(fp);
        return 0;
    }

    // Conta as linhas de dados
    while (fgets(linha, sizeof(linha), fp)) {
        count++;
    }
    fclose(fp);
    return count;

}

int main() {
    const char *nome_arquivo = "processo_043_202409032338.csv";

    
    // Conta quantos registros existem (excluindo o cabeçalho)


    int total = contar_linhas(nome_arquivo);
    printf("Número total de registros (sem cabeçalho): %d\n", total);

    // Aloca dinamicamente o vetor de Processos

    Processo *processos = malloc(total * sizeof(Processo));
    if (!processos) {
        perror("Erro ao alocar memória");
        exit(1);
    }

    int n = 0;
    printf("🔍 Lendo CSV...\n");
    ler_csv(nome_arquivo, processos, &n);
    printf("✅ Leitura finalizada. Total de processos: %d\n", n);

    printf("🔄 Ordenando por ID...\n");
    ordenar_por_id(processos, n);


    printf("🔄 Ordenando por data...\n");
    ordenar_por_data(processos, n);

    int classe = 12377;
    printf("📊 Contando classe %d...\n", classe);
    int qtd = contar_por_classe(processos, n, classe);
    printf("Classe %d tem %d processos\n", classe, qtd);


    printf("📚 Contando assuntos diferentes...\n");
    int assuntos = contar_assuntos_diferentes(processos, n);

    printf("Há %d assuntos diferentes\n", assuntos);

    printf("📄 Listando processos com mais de um assunto...\n");
    listar_varios_assuntos(processos, n);


    printf("📆 Calculando dias em tramitação...\n");
    for (int i = 0; i < n; i++) {
        printf("Processo %ld está em tramitação há %d dias\n",
               processos[i].id,
               dias_em_tramitacao(processos[i].data_ajuizamento));
    }

    free(processos);

    printf("✅ Finalizado!\n");
    return 0;
}