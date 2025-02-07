
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_warning();

int i, j;
int population, immigrants, population_starved, bushels, 
	rat_dinner, bushels_per_acre, harvest, acres, acre_cost, year;

char buf[64];

int
main()
{
	srand(time(NULL));

	printf(	"HAMURABI -\n"
			"WHERE YOU GOVERN THE ANCIENT KINGDOM OF SUMERIA.\n"
			"THE OBJECT IS TO KEEP THE KINGDOM GROWING.\n"
			"(IF YOU WANT TO QUIT, SELL ALL YOUR LAND)\n"
			"\n\n\nPRESS  ENTER  TO BEGIN YOUR REIGN");

	gets(buf);

	population=100;
	immigrants=5;
	population_starved=0;
	bushels=2800;
	rat_dinner=200;
	bushels_per_acre=3;
	harvest=3000;
	acres=1000;
	j=1;
	year=1;

	while (1) {

		printf("\n\nHAMURABI, I BEG TO REPORT THAT IN YEAR %d:\n", year);

		printf("%d %s STARVED, AND %d %s CAME TO THE CITY.\n",
			population_starved, (population_starved==1)?"PERSON":"PEOPLE",
			immigrants, (immigrants==1)?"PERSON":"PEOPLE"	);

		if (j==0) {
			population /= 2;
			printf(" THE PLAGUE KILLED HALF THE PEOPLE.\n");
		}

		printf(" THE POPULATION IS NOW %d.\n", population);
		printf(" WE HARVESTED %d BUSHELS AT %d BUSHELS PER ACRE.\n", harvest, bushels_per_acre);
		printf(" RATS DESTROYED %d BUSHELS, LEAVING %d BUSHELS IN STORAGE.\n", rat_dinner, bushels);
		printf(" THE CITY OWNS %d ACRES OF LAND.\n", acres);

		acre_cost = 16 + rand()%6;
		printf(" LAND IS WORTH %d BUSHELS PER ACRE.", acre_cost);
		printf("\n\nHAMURABI . . .\n");

_buy_again:
		printf("      BUY HOW MANY ACRES?");
		gets(buf);
		i = atol(buf);
		if (i!=0) {

			j=i*acre_cost;
			if (j>bushels) {
				print_warning();
				goto _buy_again;
			}

			bushels -= j;
			acres += i;
		}

		printf("   *  YOU ARE BUYING %d ACRES.                         \n", i);

		if (!i) {

_sell_again:
			printf("      SELL HOW MANY ACRES?");
			gets(buf);
			i = atol(buf);
			if (i==acres)
				break;
			if (i!=0 && i>=acres) {
				print_warning();
				goto _sell_again;
			}
			printf("   *  YOU ARE SELLING %d ACRES.                    \n", i);

			acres -= i;
			bushels += acre_cost*i;

		}

_distribute_again:
		printf("      HOW MANY BUSHELS SHALL WE DISTRIBUTE AS FOOD?");
		gets(buf);
		i = atol(buf);
		if (i>bushels) {
			print_warning();
			goto _distribute_again;
		}

		bushels -= i;
		population_starved = population-i/20;
		immigrants = 0;
		if (population_starved<0) {
			immigrants = (-population_starved)/2;
			population_starved = 0;
		}
		printf("   *  YOU ARE DISTRIBUTING %d BUSHELS.                \n", i);

_plant_again:
		printf("      HOW MANY ACRES SHALL WE PLANT?");
		gets(buf);
		i = atol(buf);
		j = i/2;
		if (i>acres || j>bushels || i>population*10) {
			print_warning();
			goto _plant_again;
		}

		printf("\n\n-----------\n\n");
		bushels -= j;
		bushels_per_acre = (rand()%5)+1;
		harvest = bushels_per_acre * i;
		rat_dinner = (int)((float)(bushels+harvest) * 0.7 * ((rand()%100)/100.0));
		bushels = bushels - rat_dinner + harvest;
		j = rand()%11 - 1;
		immigrants += (5-bushels_per_acre)*bushels/600+1;
		immigrants = (immigrants>50)?50:immigrants;
		immigrants = (immigrants<0)?0:immigrants;
		population += immigrants - population_starved;
		year++;
	}

	printf("\n\n\n\n\nYOU HAVE SOLD ALL YOUR LAND.\n");
	printf("\nTHE GAME IS OVER, AND YOU CAN GO BACK TO BEING YOURSELF.\n");
	printf("BYE\n");
	return 0;
}

void
print_warning()
{
	printf("-->   HAMURABI!  THINK AGAIN -- YOU ONLY HAVE                \n");
	printf("-->   %d PEOPLE,  %d ACRES, AND %d BUSHELS IN STOREHOUSES.\n", population, acres, bushels);
}

