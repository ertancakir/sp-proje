#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

char *dosya_oku(char *dosya, int boyut){
	char *veri = malloc(sizeof(char) * boyut);
	char ch;
	FILE *fp;
	fp = fopen(dosya, "r");
	int i = 0;
	while ((ch = fgetc(fp)) != EOF){
		veri[i] = ch;
		i++;
	}
	fclose(fp);
	return veri;
}
void dosya_yaz(char **files, char **content , char * output, int dosya_adet){
	FILE * fp;
	fp = fopen(output, "w+");

	for(int i=0;i<dosya_adet;i++){
		fprintf(fp,files[i]);
	}
	for(int i=0;i<dosya_adet;i++){
		fprintf(fp,content[i]);
	}
	fclose(fp);
}

int komut_calistir(char **args){
	int i;

	if (args[0] == NULL){
		// An empty command was entered.
		return 1;
	}
	else if (strcmp(args[0], "tarsau") == 0){
		if (strcmp(args[1], "b") == 0){
			int konum = 2;
			int toplam_boyut = 0;
			char **files = malloc(sizeof(char *) * 32);
			char **content = malloc(sizeof(char *) * 32);

			while (strcmp(args[konum], "o") != 0){
				struct stat st;
				stat(args[konum], &st);
				int size = (int)st.st_size;

				char dosya[32];
				sprintf(dosya, "|%s,%d,%d|", args[konum], 0, size);
				files[konum - 2] = strdup(dosya);
				content[konum - 2] = dosya_oku(args[konum],size);

				konum++;
			}
			int dosya_adet = konum - 2;
			if (dosya_adet >= 32){
				printf("Girdi dosyaları 32'den fazla olamaz");
				free(files);
				return 0;
			}
			dosya_yaz(files,content,args[konum + 1],dosya_adet);
			return 1;
		}
		else if (strcmp(args[1], "a") == 0){
			printf("tarsau a komutu\n");
		}
		return 1;
	}
}

#define LSH_RL_BUFSIZE 1024
/**
   @brief Read a line of input from stdin.
   @return The line from stdin.
 */
char *term_satir_oku(void){
	int bufsize = LSH_RL_BUFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize);
	int c;

	if (!buffer){
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	while (1){
		// Read a character
		c = getchar();

		if (c == EOF){
			exit(EXIT_SUCCESS);
		}
		else if (c == '\n'){
			buffer[position] = '\0';
			return buffer;
		}
		else{
			buffer[position] = c;
		}
		position++;

		// If we have exceeded the buffer, reallocate.
		if (position >= bufsize){
			bufsize += LSH_RL_BUFSIZE;
			buffer = realloc(buffer, bufsize);
			if (!buffer){
				fprintf(stderr, "lsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " -"
/**
   @brief Split a line into tokens (very naively).
   @param line The line.
   @return Null-terminated array of tokens.
 */
char **term_satir_parcala(char *line){
	int bufsize = LSH_TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token, **tokens_backup;

	if (!tokens){
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, LSH_TOK_DELIM);
	while (token != NULL){
		tokens[position] = token;
		position++;

		if (position >= bufsize){
			bufsize += LSH_TOK_BUFSIZE;
			tokens_backup = tokens;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			if (!tokens){
				free(tokens_backup);
				fprintf(stderr, "lsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, LSH_TOK_DELIM);
	}
	tokens[position] = NULL;
	return tokens;
}

int main(){
	char *satir;
	char **args;
	int durum;

	do{
		printf("> ");
		satir = term_satir_oku();
		args = term_satir_parcala(satir);
		durum = komut_calistir(args);
	} while (durum);
	free(satir);
	free(args);
	return 0;
}
