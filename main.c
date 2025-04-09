#include <stdio.h>
#include "processo.h"

int main() {
    Processo processos[MAX_PROCESSOS];
    int quantidade;

    // Caminho completo para o arquivo CSV (com aspas e nome do arquivo)
    const char *caminho_csv = "processo_043_202409032338.csv";


    // Carrega os processos
    if (!carregar_processos(caminho_csv, processos, &quantidade)) {
        printf("Erro ao carregar o arquivo CSV.\n");
        return 1;
    }

    // 1. Ordenar por ID crescente
    ordenar_por_id(processos, quantidade);
    salvar_csv("ordenado_por_id.csv", processos, quantidade);
    printf("Arquivo 'ordenado_por_id.csv' salvo.\n");

    // 2. Ordenar por data decrescente
    ordenar_por_data(processos, quantidade);
    salvar_csv("ordenado_por_data.csv", processos, quantidade);
    printf("Arquivo 'ordenado_por_data.csv' salvo.\n");

    // 3. Contar processos por id_classe
    int id_classe = 12554;
    int count = contar_por_classe(processos, quantidade, id_classe);
    printf("Quantidade de processos com id_classe %d: %d\n", id_classe, count);

    // 4. Contar id_assunto únicos
    int total_unicos = contar_assuntos_unicos(processos, quantidade);
    printf("Quantidade de id_assunto únicos: %d\n", total_unicos);

    // 5. Listar processos com mais de um assunto
    printf("Processos com mais de um assunto:\n");
    listar_multiplos_assuntos(processos, quantidade);

    // 6. Dias em tramitação (exemplo com o primeiro processo)
    int dias = dias_em_tramitacao(processos[0]);
    printf("Dias em tramitação do primeiro processo: %d\n", dias);

    return 0;
}
