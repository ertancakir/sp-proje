#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char *term_satir_oku(void);
char **term_satir_parcala(char *line, char * token_delimeter);
char *dosya_oku(char *dosya, int boyut);
void dosya_yaz(char **files, char **content , char * output, int dosya_adet);
int komut_calistir(char **args);


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

	if (args[0] == NULL){
		// Bos Komut Girildi
		return 1;
	}
	else if (strcmp(args[0], "tarsau") == 0){
		if (strcmp(args[1], "b") == 0){
			int konum = 2;
			char **files = malloc(sizeof(char *) * 32);
			char **content = malloc(sizeof(char *) * 32);

			while (args[konum] != NULL && strcmp(args[konum], "o") != 0){
				char * uzanti = strrchr(args[konum],'.');
				if(uzanti != NULL && uzanti != ".txt"){
					printf("%s giris dosyasi bicimi uyumsuz!\n", args[konum]);
					free(files);
					free(content);
					return 0;
				}
				struct stat st;
				stat(args[konum], &st);
				int size = (int)st.st_size;
				int perm = st.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
				char dosya[32];
				sprintf(dosya, "%s,%o,%d|", args[konum], perm, size);
				files[konum - 2] = strdup(dosya);
				content[konum - 2] = dosya_oku(args[konum],size);

				konum++;
			}
			int dosya_adet = konum - 2;
			if (dosya_adet >= 32){
				printf("Girdi dosyaları 32'den fazla olamaz");
				free(files);
				free(content);
				return 0;
			}
			if(args[konum] == NULL){
				dosya_yaz(files,content,"a.sau",dosya_adet);
			}
			else{
				char * uzanti = strrchr(args[konum + 1],'.');
				printf("%s\n",uzanti);
				if(strcmp(uzanti,".sau") != 0){
					printf("%s cikis bicimi uyumsuz!\n",args[konum + 1]);
					free(files);
					free(content);
					return 0;
				}
				dosya_yaz(files,content,args[konum + 1],dosya_adet);
			}
			return 1;
		}
		else if (strcmp(args[1], "a") == 0){
			char * uzanti = strrchr(args[2],'.');
			if(strcmp(uzanti,".sau") != 0){
				printf("%s giris bicimi uyumsuz!\n",args[2]);
				return 0;
			}
			mkdir(args[3], 0777);
			struct stat st;
			stat(args[2], &st);
			int size = (int)st.st_size;

			char * veri = dosya_oku(args[2], size);
			char ** veri_args = term_satir_parcala(veri, "|");
			
			int len = 0;
			while(veri_args[len] != NULL){
				len++;
			}
			int konum = 0;

			char * arsiv_veri = veri_args[len-1];

			for(int i=0;i<len-1;i++){
				char * tmp = veri_args[i];
				char ** dosya = term_satir_parcala(tmp, ",");
				char * dosya_verisi = malloc(sizeof(char) * atoi(dosya[2]));
				char path[20];
				sprintf(path,"%s/%s",args[3],dosya[0]);
				FILE * fp;
				fp = fopen(path, "w+");
				int sayac = 0;
				for(int j=konum;j<atoi(dosya[2]) + konum;j++){
					dosya_verisi[sayac] = arsiv_veri[j];
					sayac++;
				}
				fprintf(fp,dosya_verisi);
				fclose(fp);
				konum += atoi(dosya[2]);
				free(dosya_verisi);
				free(dosya);
			}
			free(veri);
			free(veri_args);
		}
		return 1;
	}
}

#define LSH_RL_BUFSIZE 1024

char *term_satir_oku(void){
	int bufsize = LSH_RL_BUFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize);
	int c;

	if (!buffer){
		fprintf(stderr, "\n");
		exit(EXIT_FAILURE);
	}

	while (1){
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

		if (position >= bufsize){
			bufsize += LSH_RL_BUFSIZE;
			buffer = realloc(buffer, bufsize);
			if (!buffer){
				fprintf(stderr, "\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}

#define LSH_TOK_BUFSIZE 64

char **term_satir_parcala(char *line, char * token_delimeter){
	int bufsize = LSH_TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token, **tokens_backup;

	if (!tokens){
		fprintf(stderr, "\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, token_delimeter);
	while (token != NULL){
		tokens[position] = token;
		position++;

		if (position >= bufsize){
			bufsize += LSH_TOK_BUFSIZE;
			tokens_backup = tokens;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			if (!tokens){
				free(tokens_backup);
				fprintf(stderr, "\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, token_delimeter);
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
		args = term_satir_parcala(satir," -");
		durum = komut_calistir(args);
	} while (durum);
	free(satir);
	free(args);
	
	return 0;
}
