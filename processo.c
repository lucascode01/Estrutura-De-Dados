#include "processo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Substituto para strptime no Windows (agora compatível com milissegundos)
int parse_datetime(const char *datetime_str, struct tm *tm) {
    memset(tm, 0, sizeof(struct tm));

    // Cria cópia da string original e corta após o ponto (milissegundos)
    char buffer[30];
    strncpy(buffer, datetime_str, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    char *ponto = strchr(buffer, '.');
    if (ponto != NULL) {
        *ponto = '\0';
    }

    int r = sscanf(buffer, "%d-%d-%d %d:%d:%d",
                   &tm->tm_year, &tm->tm_mon, &tm->tm_mday,
                   &tm->tm_hour, &tm->tm_min, &tm->tm_sec);
    if (r != 6) return 0;

    tm->tm_year -= 1900;
    tm->tm_mon -= 1;
    return 1;
}

int carregar_processos(const char *filename, Processo processos[], int *quantidade) {
    FILE *arquivo = fopen(filename, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    char linha[1024];
    int i = 0;

    // Lê o cabeçalho
    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo) && i < MAX_PROCESSOS) {
        char *token = strtok(linha, ",");

        if (token) processos[i].id = atoi(token);
        token = strtok(NULL, ",");
        if (token) processos[i].id_classe = atoi(token);
        token = strtok(NULL, ",");
        if (token) processos[i].id_assunto = atoi(token);
        token = strtok(NULL, ",");
        if (token) strncpy(processos[i].data_ajuizamento, token, sizeof(processos[i].data_ajuizamento));
        token = strtok(NULL, ",");
        if (token) strncpy(processos[i].assuntos_extra, token, sizeof(processos[i].assuntos_extra));

        i++;
    }

    fclose(arquivo);
    *quantidade = i;
    return 1;
}

void salvar_csv(const char *filename, Processo processos[], int quantidade) {
    FILE *arquivo = fopen(filename, "w");
    if (!arquivo) {
        perror("Erro ao salvar o arquivo");
        return;
    }

    fprintf(arquivo, "id,id_classe,id_assunto,data_ajuizamento,assuntos_extra\n");

    for (int i = 0; i < quantidade; i++) {
        fprintf(arquivo, "%d,%d,%d,%s,%s\n",
                processos[i].id,
                processos[i].id_classe,
                processos[i].id_assunto,
                processos[i].data_ajuizamento,
                processos[i].assuntos_extra);
    }

    fclose(arquivo);
}

void ordenar_por_id(Processo processos[], int quantidade) {
    for (int i = 0; i < quantidade - 1; i++) {
        for (int j = i + 1; j < quantidade; j++) {
            if (processos[i].id > processos[j].id) {
                Processo tmp = processos[i];
                processos[i] = processos[j];
                processos[j] = tmp;
            }
        }
    }
}

void ordenar_por_data(Processo processos[], int quantidade) {
    for (int i = 0; i < quantidade - 1; i++) {
        for (int j = i + 1; j < quantidade; j++) {
            if (strcmp(processos[i].data_ajuizamento, processos[j].data_ajuizamento) < 0) {
                Processo tmp = processos[i];
                processos[i] = processos[j];
                processos[j] = tmp;
            }
        }
    }
}

int contar_por_classe(Processo processos[], int quantidade, int id_classe) {
    int count = 0;
    for (int i = 0; i < quantidade; i++) {
        if (processos[i].id_classe == id_classe) {
            count++;
        }
    }
    return count;
}

int contar_assuntos_unicos(Processo processos[], int quantidade) {
    int unicos[MAX_PROCESSOS];
    int total = 0;

    for (int i = 0; i < quantidade; i++) {
        int id = processos[i].id_assunto;
        int encontrado = 0;
        for (int j = 0; j < total; j++) {
            if (unicos[j] == id) {
                encontrado = 1;
                break;
            }
        }
        if (!encontrado) {
            unicos[total++] = id;
        }
    }

    return total;
}

void listar_multiplos_assuntos(Processo processos[], int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        if (strchr(processos[i].assuntos_extra, ';')) {
            printf("ID: %d | Assuntos extra: %s\n", processos[i].id, processos[i].assuntos_extra);
        }
    }
}

int dias_em_tramitacao(Processo processo) {
    struct tm data_ini;
    if (!parse_datetime(processo.data_ajuizamento, &data_ini)) {
        return -1; // erro na conversão da data
    }

    time_t inicio = mktime(&data_ini);
    time_t agora = time(NULL);

    if (inicio == -1) return -1;

    double diff = difftime(agora, inicio);
    return (int)(diff / (60 * 60 * 24));
}

