#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "processo.h"

void ler_csv(const char *nome_arquivo, Processo processos[], int *n) {
    FILE *fp = fopen(nome_arquivo, "r");
    if (!fp) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    char linha[512];
    fgets(linha, sizeof(linha), fp); // pula cabeçalho

    *n = 0;
    while (fgets(linha, sizeof(linha), fp)) {
        Processo *p = &processos[*n];
        char *token = strtok(linha, ",");
        p->id = atol(token);

        token = strtok(NULL, ",");
        strcpy(p->numero, token);

        token = strtok(NULL, ",");
        strcpy(p->data_ajuizamento, token);

        token = strtok(NULL, ",");
        p->num_classes = sscanf(token, "{%d}", &p->id_classe[0]);

        token = strtok(NULL, ",");
        p->num_assuntos = sscanf(token, "{%d}", &p->id_assunto[0]);

        token = strtok(NULL, ",");
        p->ano_eleicao = atoi(token);

        (*n)++;
    }
    fclose(fp);
}

void ordenar_por_id(Processo processos[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (processos[i].id > processos[j].id) {
                Processo temp = processos[i];
                processos[i] = processos[j];
                processos[j] = temp;
            }
        }
    }

    FILE *fp = fopen("ordenado_por_id.csv", "w");
    fprintf(fp, "id,numero,data_ajuizamento,id_classe,id_assunto,ano_eleicao\n");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%ld,%s,%s,{%d},{%d},%d\n",
            processos[i].id, processos[i].numero, processos[i].data_ajuizamento,
            processos[i].id_classe[0], processos[i].id_assunto[0], processos[i].ano_eleicao);
    }
    fclose(fp);
}

void ordenar_por_data(Processo processos[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (strcmp(processos[i].data_ajuizamento, processos[j].data_ajuizamento) < 0) {
                Processo temp = processos[i];
                processos[i] = processos[j];
                processos[j] = temp;
            }
        }
    }

    FILE *fp = fopen("ordenado_por_data.csv", "w");
    fprintf(fp, "id,numero,data_ajuizamento,id_classe,id_assunto,ano_eleicao\n");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%ld,%s,%s,{%d},{%d},%d\n",
            processos[i].id, processos[i].numero, processos[i].data_ajuizamento,
            processos[i].id_classe[0], processos[i].id_assunto[0], processos[i].ano_eleicao);
    }
    fclose(fp);
}

int contar_por_classe(Processo processos[], int n, int classe) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processos[i].num_classes; j++) {
            if (processos[i].id_classe[j] == classe) {
                count++;
                break;
            }
        }
    }
    return count;
}

int contar_assuntos_diferentes(Processo processos[], int n) {
    int unicos[1000];
    int total = 0;

    for (int i = 0; i < n; i++) {
        int id = processos[i].id_assunto[0];
        int existe = 0;
        for (int j = 0; j < total; j++) {
            if (unicos[j] == id) {
                existe = 1;
                break;
            }
        }
        if (!existe) {
            unicos[total++] = id;
        }
    }
    return total;
}

void listar_varios_assuntos(Processo processos[], int n) {
    for (int i = 0; i < n; i++) {
        if (processos[i].num_assuntos > 1) {
            printf("Processo %ld tem %d assuntos\n", processos[i].id, processos[i].num_assuntos);
        }
    }
}

int dias_em_tramitacao(const char *data_str) {
    struct tm tm = {0};
    char data_formatada[20];

    // Copia só os primeiros 19 caracteres: "YYYY-MM-DD HH:MM:SS"
    strncpy(data_formatada, data_str, 19);
    data_formatada[19] = '\0';

    strptime(data_formatada, "%Y-%m-%d %H:%M:%S", &tm);
    time_t data_ajuizamento = mktime(&tm);
    time_t agora = time(NULL);
    double segundos = difftime(agora, data_ajuizamento);
    return (int)(segundos / (60 * 60 * 24));
}
