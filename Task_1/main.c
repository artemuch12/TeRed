#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>


int main()
{
        off_t Poz;
        off_t Poz1;
        int i = 0;
        int file_deskrip;
        char name[20];
        char buff[255];
        char char_buff;
        printf("Введите имя файла\n");
        scanf("%s", name);

        file_deskrip = open(name, O_RDONLY);
        if (file_deskrip < 1)
        {
                puts("Ошибка! Файл не найден. Файл с таким именем будет создан.");
                printf("Введите текст\n");
                scanf("%s", buff);
                file_deskrip = open(name, O_CREAT, 0666);
                close(file_deskrip);
                file_deskrip = open(name, O_WRONLY);
                write(file_deskrip, buff, strlen(buff));
                close(file_deskrip);
        }
        else
        {
                puts("Файл открыт.");
                Poz = lseek(file_deskrip, -1, SEEK_END );
                for(i = 0; i <= Poz; i++)
                {
                        read(file_deskrip, &char_buff, 1);
                        printf("%c", char_buff);
                        if (i != Poz)
                        {
							Poz1 = lseek(file_deskrip, -2, SEEK_CUR);
						}
                        if(Poz1 == -1)
                        {
                                printf("\n");
                                puts("Внимание! Выход за границы файла.");
                                exit(-1);
                        }
                }
                printf("\n");
                close(file_deskrip);
        }
}




