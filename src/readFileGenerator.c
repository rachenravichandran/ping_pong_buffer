#include<stdio.h>

int main(void)
{
    FILE *fptr = fopen("read.txt","w");
    unsigned int MAX_LINES;
    unsigned int MAX_WIDTH;
    int i = 0, j = 0;
    printf("Enter the number of lines : ");
    scanf("%d",&MAX_LINES);
    printf("Enter the number of datas in each line : ");
    scanf("%d",&MAX_WIDTH);
    while(i < MAX_LINES)
    {
    	for(j = 0; j < MAX_WIDTH; j++)
    	{
    		fprintf(fptr,"%d",rand()%100);
    		if ( j < MAX_WIDTH - 1)
    		{
    			fprintf(fptr," ");
    		}
    	}
        if(i < MAX_LINES -1)
        {
        	fprintf(fptr,"\n");
    	}
        i++;
    }
    printf("File write to 'read.txt' complete\n");
    fclose(fptr);
}
