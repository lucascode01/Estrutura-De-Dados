#include <stdio.h>
#include "processo.h"

#define MAX_PROCESSOS 1000

int main() {
    Processo processos[MAX_PROCESSOS];
    int total = 0;

    printf("🔍 Lendo CSV...\n");
    ler_csv("processo_043_202409032338.csv", processos, &total);
    printf("✅ Leitura finalizada. Total de processos: %d\n", total);

    printf("🔄 Ordenando por ID...\n");
    ordenar_por_id(processos, total);

    printf("🔄 Ordenando por data...\n");
    ordenar_por_data(processos, total);

    int classe = 12377;
    printf("📊 Contando classe %d...\n", classe);
    int qtd = contar_por_classe(processos, total, classe);
    printf("Classe %d tem %d processos\n", classe, qtd);

    printf("📚 Contando assuntos diferentes...\n");
    int assuntos = contar_assuntos_diferentes(processos, total);
    printf("Há %d assuntos diferentes\n", assuntos);

    printf("📄 Listando processos com mais de um assunto...\n");
    listar_varios_assuntos(processos, total);

    printf("📆 Calculando dias em tramitação...\n");
    for (int i = 0; i < total; i++) {
        printf("Processo %ld está em tramitação há %d dias\n",
               processos[i].id,
               dias_em_tramitacao(processos[i].data_ajuizamento));
    }

    printf("✅ Finalizado!\n");
    return 0;
}