#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct serie {
	char serieName[20];
	int numSeasons;
	int rank;
	int* watchingDetails;
	int code;
}serie;

int showMenu();
int unifyDatabase(FILE*, FILE*, int*);
serie** getUnifyDatabaseDetails(int);
void printSeriesList(serie**,int);
void getWatchingDetails(serie**,int);
void printWatchingDetails();
void printSeriesByCode(serie**);
void printSeriesByRank(serie**, int);
void makeSeriesFileSortedByRank(serie**, int);
int inserie(serie**, int,int);
serie* create_serial(char*, int*,int);
int find_max_rank(serie**, int);
int find_rank(serie** p, int count,int rank);
void Exit(serie**, int);






int main() {
	
	FILE *f1, *f2;
	int select;
	char input_f1[100];
	char input_f2[100];
	int count=0;
	int flag = 1;
	serie** un =NULL;

	while (1)
	{
		showMenu();
		fscanf(stdin, "%d", &select);

		while (flag)
		{
			if (select == 1)
			{
				flag = 0;
				break;
			}

			printf("You must unify database before using this option!!!\n");
			showMenu();
			scanf("\n%d", &select);
			if (select == 1)
				flag = 0;

		}


		switch (select)
		{
		case 1:
		{
			printf("Enter First File Name:\n");
			fscanf(stdin, "%s", input_f1);
			while (!(f1 = fopen(input_f1, "r")))
			{
				printf("error in opening file %s !!!\n", input_f1);
				printf("Enter First File Name:\n");
				fscanf(stdin, "%s", input_f1);
			}
			printf("Enter Second File Name:\n");
			fscanf(stdin, "%s", input_f2);
			while (!(f2 = fopen(input_f2, "r")))
			{
				printf("error in opening file %s !!!\n", input_f2);
				printf("Enter Second File Name:\n");
				fscanf(stdin, "%s", input_f2);
			}
			unifyDatabase(f1, f2, &count);
			break;
		}
		case 2:
		{
			un = getUnifyDatabaseDetails(count);
			break;
		}
		case 3:
		{
			printSeriesList(un, count);
			break;
		}
		case 4:
		{
			getWatchingDetails(un, count);
			break;
		}
		case 5:
		{   printWatchingDetails();
		break;
		}
		case 6:
		{   printSeriesByRank(un, count);
		break;
		}
		case 7:
		{   makeSeriesFileSortedByRank(un, count);
		break;
		}
		case 8:
		{   Exit(un, count);
		break;
		}
	deafult:


		;
		}
	
	}
	
	

		
		//printf("heyyyy:line %s,%d,%d\n", un[0]->serieName, un[0]->numSeasons, un[0]->rank);
		
	


	




	return 0;
}

int showMenu()
{
	printf("**********Main Menu **************\n");
	printf("1. Unify Database.\n");
	printf("2. Get Unify Database Details.\n");
	printf("3. Print The Series List Sorted By Code.\n");
	printf("4. Get Watching Details.\n");
	printf("5. Show All Watching Details.\n");
	printf("6. Print Series Sorted By Rank.\n");
	printf("7. Make Series File Sorted By Rank.\n");
	printf("8. Exit.\n");
	printf("Enter Your Selection:");
	return 1;
}

int unifyDatabase(FILE* f, FILE* p,int *count)
{
	FILE* fp;
    fp = fopen("series.txt", "w");
	int success_f, success_p;
	char f_serial[20], p_serial[20];
	int  code_f, code_p;
	int f_num_series, p_num_series;
	char f_active, p_active;
	success_f = fscanf(f, "%[^,]%*c%3d%*c%2d%*c%c%*c", f_serial, &code_f, &f_num_series, &f_active);//[name,code,series_num,activ[Y/N]
	success_p = fscanf(p, "%[^,]%*c%3d%*c%2d%*c%c%*c", p_serial, &code_p, &p_num_series, &p_active);
	while (success_f!=EOF && success_p!=EOF)//need to optimize
	{


		if (success_f == EOF)
			code_f = 1000;
		if (success_p == EOF)
			code_p = 1000;
		
			if (code_f < code_p)
			{
				if (f_active == 'Y')
				{
					fprintf(fp, "%s,%d,%d\n", f_serial, code_f, f_num_series);
					(*count)++;
				}
				success_f = fscanf(f, "%[^,]%*c%3d%*c%2d%*c%c%*c", f_serial, &code_f, &f_num_series, &f_active);//[name,code,series_num,activ[Y/N]
				continue;
			}
			if (code_f > code_p)
			{
				if (p_active == 'Y') {
					
					fprintf(fp, "%s,%d,%d\n", p_serial, code_p, p_num_series);
					(*count)++;
				}
				
				success_p = fscanf(p, "%[^,]%*c%3d%*c%2d%*c%c%*c", p_serial, &code_p, &p_num_series, &p_active);
				continue;	
			}
			if (code_f == code_p)
			{
				

				if ((f_active == 'Y') && (p_active == 'Y'))
				{
					if (f_num_series > p_num_series)
					{
						
						fprintf(fp, "%s,%d,%d\n", f_serial, code_f, f_num_series);
						(*count)++;
					}

					else
					{
						
						fprintf(fp, "%s,%d,%d\n", p_serial, code_p, p_num_series);
						(*count)++;
					}

				}

				else if ((f_active == 'N') && (p_active == 'Y'))
				{
					
					fprintf(fp, "%s,%d,%d\n", p_serial, code_p, p_num_series);
					(*count)++;
				}

				else if ((f_active == 'Y') && (p_active == 'N'))
				{
					
					fprintf(fp, "%s,%d,%d\n", f_serial, code_f, f_num_series);
					(*count)++;
				}
				
				success_f = fscanf(f, "%[^,]%*c%3d%*c%2d%*c%c%*c", f_serial, &code_f, &f_num_series, &f_active);//[name,code,series_num,activ[Y/N]
				success_p = fscanf(p, "%[^,]%*c%3d%*c%2d%*c%c%*c", p_serial, &code_p, &p_num_series, &p_active);//[name,code,series_num,activ[Y/N]
				
				continue;
			}
			
			
		}
	
	printf("Unify Succeeded\n");
	return fclose(fp);
	
}


serie* make_series(char* name,int series,int code)
{
	serie* serial;
	serial = (serie*)malloc(sizeof(serie));
	if (serial == NULL)
	{
		printf("error!");
		exit(1);
	}
	strcpy(serial->serieName, name);
	serial->rank = 0;
	serial->numSeasons = series;
	serial->watchingDetails = (int*)malloc(series * sizeof(int));
	int i;
	for (int i = 0;i < series; i++)
		serial->watchingDetails[i] = 0;
	serial->code = code;
	return serial
;}

serie** getUnifyDatabaseDetails(int count)
{   
	FILE *fp = fopen("series.txt", "r");
	FILE* watch = fopen("watching.txt", "w");
	int sc;
	char name[20];
	int code,series;
	serie** unify = (serie**)malloc(count * sizeof(serie*));
	if (unify == NULL)
	{
		printf("error in dynamic allocation\n");
		exit(1);
	}
	int i = 0;
	while (sc = fscanf(fp, "%[^,]%*c%3d%*c%2d%*c", name, &code, &series) != EOF)
	{
		unify[i] = make_series(name, series,code);
		i++;
	}
	fclose(fp);
	fclose(watch);
	return unify;
}


void printSeriesList(serie** p,int count)
{
	int i;	
	printf("Serie List");
	for (i = 0;i < count; i++)
	{
		printf("%03d-%s(%d)\n",p[i]->code, p[i]->serieName, p[i]->numSeasons);
	}
}

int inserie(serie** s, int count,int code)
{
	int i;
	for (i = 0;i < count;i++)
	{
		if (code == s[i]->code)
			return i;
		
	}
	return -1;
	
}
void getWatchingDetails(serie** p, int count)
{
	FILE* file = fopen("watching.txt", "a");
	
	int user_numseason;
	int index;
	int flag = 1;
	printf("Insert Serie Code:");

	while (flag)
	{

		{   int user_code;
			int suc = fscanf(stdin, "%d", &user_code);
			printf("%d", suc);
			if (suc == 0 || (suc == EOF))
			{
				printf("wrong serie code, try again!!!\n");
				continue;
			}
			index = inserie(p, count, user_code);
			if (index == -1)
			{
				printf("wrong serie code, try again!!!\n");
				continue;
			}

			else
			{
				printf("index is:%d", index);
				printf("Insert Season Number(1-%d)\n", p[index]->numSeasons);
				do {
					int succ = scanf("%d", &user_numseason);
					if (succ == 0)
					{
						printf("wrong season number, try again!!!\n");
						continue;
					}
					else if (p[index]->numSeasons < user_numseason)
					{
						printf("wrong season number, try again!!!\n");
						continue;
					}
					else {
						int j;
						for (j = 0;j < p[index]->numSeasons; j++)
						{
							if (user_numseason == j + 1)
							{
								p[index]->watchingDetails[j]+=1;
								p[index]->rank++;
								//printf("p[index]: %d\np[index]->watchingdetails: %d\n Rank:%d\n", p[index]->code, p[index]->watchingDetails[j], p[index]->rank);
								fprintf(file, "Series Code: %03d, Season Number:%d\n", p[index]->code, p[index]->watchingDetails[j]);
								flag = 0;
								printf("series: %s, season: %d accepted!!!\n", p[index]->serieName, j + 1);
								break;
							}


						}

					}


				} while (flag);


			}



		}

		fclose(file);
	}
}

void printWatchingDetails()
{
	FILE* fp = fopen("watching.txt", "r");
	int i;
	printf("Watching Details\n");
	char line[50];
	while (fgets(line,50,fp))
	{
		printf("%s\n",line);
	}
	rewind(fp);
	fclose(fp);

	

}

int find_max_rank(serie** p, int count)
{   
	int i;
	int max = 0;
	int s;
	for (i = 0;i < count;i++)
	{
	
		if (p[i]->rank > max)
		{
			max = p[i]->rank;
			s = i;
		}	    
	}
	return max;
}


int find_rank(serie** p, int count, int rank)
{
	int i;
	int max = 0;
	int s;
	for (i = 0;i < count;i++)
	{
		if (p[i]->rank == rank)
		{
			return i;
		}
	}
	return -1;
}
 void printSeriesByRank(serie** p , int count)
{
	//printf("enterrrr\n");
	int i,j;
	int max = find_max_rank(p,count);
	
	for (i = max;i >= 0;i--)
	{
		printf("RANK %d\n", i);
		for (j = 0;j < count;j++)
		{

			if (i == p[j]->rank)
			{
				printf("%03d-%s\n", p[j]->code, p[j]->serieName);
				for (int k = 0;k < p[j]->numSeasons;k++)
				{
					printf("season %d:%d\n", k + 1, p[j]->watchingDetails[k]);
				}
			}
		}

	}
}

 void makeSeriesFileSortedByRank(serie** p, int count)
 {
	 FILE* fp = fopen("seriesRank.txt", "w");
	 int i, j;
	 int max = find_max_rank(p, count);
	 
	 for (i = max;i >= 0;i--)
	 {
		 for (j = 0;j < count;j++)
		 {

			 if (i == p[j]->rank)
				 fprintf(fp,"%03d(Rank:%d)-%s\n", p[j]->code, p[j]->rank,p[j]->serieName); 
		 }

	 }
	 fclose(fp);
 }

 void Exit(serie** p, int count)
 {
	 int i;
	 for (i = 0;i < count;i++)
		 free(p[i]->watchingDetails[0]);
	    
	 free(p);


 }