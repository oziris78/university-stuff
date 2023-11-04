#include <stdio.h>
#define row 10
#define col 10
#define length 100

int main()
{
    int n,m,i,j,textIndex=0;
    char text[length];
    int image[row][col];
    
    printf("Enter Image Size: N,M respectively \n");
    scanf("%d%d",&n,&m);
    
    printf("Enter The Pixel Values from top-left to bottom-right\n");
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            scanf("%d", &image[i][j]);
        }
    }

    
    for (i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            text[textIndex]=(char)image[i][j];
            textIndex++;
        }
    }

    printf("The Text That Was Hidden Into The Image is: %s\n",text);

    
    return 0;
}
