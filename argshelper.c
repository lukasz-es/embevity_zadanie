#include "argshelper.h"

void update_min_count_value(const char *instr, int *x)
{
    int tmp;
	
	if (instr!=NULL)
    {
		tmp=atoi(instr);
		
		if( (tmp > 1) && (tmp < 64) )
		{
			*x = tmp;
		}
		else
		{
			printf("Minimum count is out of range [2 - 63], default used\n");
		}
    }
}

void update_threshold_count_value(const char *instr, float *x)
{
    if (instr!=NULL)
    {
		*x=atof(instr);
    }
}

void parseArgs(int argc, char **argv, int *min_count, float *threshold)
{
    int c;
	
    while((c=getopt(argc,argv,":t:n:h")) != -1)
    {
        switch (c)
        {
        case 't':
            update_threshold_count_value(optarg, threshold);
            break;

        case 'n':
            update_min_count_value(optarg, min_count);
            break;

        case ':':
			if (optopt != 'h')
			{
				printf("Option -%c requires argument\n", optopt);
			}
            break;

        case '?':
            printf("Option -%c is invalid\n", optopt);
            break;
			
		case 'h':
			printf("Params:\n-t: Threshold value, float, in G units\n-n: Minimum sample count, int\n    Range: 2 - 63\n\n");
			exit (0);
			break;

        default:
            ;
            break;
        }
    }
	
	printf("Working parameters:\nThreshold: %f G\nMinimum count: %d\n\n", *threshold, *min_count);
}
