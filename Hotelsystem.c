/**
 * @defgroup   CPT109 Assessment 3
 *
 * @brief      Hotel Management System
 * 
 * @author     Lenghan Zhu
 * @date       2021
 * @version    18
 * 
 * @copyright  2021 Lenghan Zhu 100% completed
 * 
 * @test       Thanks to Yuzhe Sun & Kai Ma
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/**
 * @brief      Structure for a single book
 */
struct booking
{
	int paid;
	int room;
	int year;
	int month;
	int day;
	int period;
};

/**
 * @brief       Transform serial number of customer account to account number in
 *              char form
 * @param       acc   char form account number
 * @param[in]   num   serial number
 * @return      char form account for multiple usage */
char* numtoacc(char *acc, int num)
{
	int i;
	for (i=0;i<8;i++)
		acc[7-i]=(num/(int) pow(10,i)+10)%10+48;
	acc[8]='\0';
	return acc;
}

/**
 * @brief       Transform serial number of room to char form room number
 * @param       roo   char form room number
 * @param[in]   num   room serial number
 * @return      char form room number for multiple usage */
char* numtoroo(char *roo, int num)
{
	   
	roo[0]=num/10+49;
	roo[3]='\0';
	if ((num+10)%10!=9)
	{
	   roo[1]=48;
	   roo[2]=(num+10)%10+49;
	}
	else
	{
	    roo[1]=49;
	    roo[2]=48;
	}
	return roo;
}

/**
 * @brief       Transform char form room number to serial number
 * @param       roo   char form room number
 * @return      -serial room number --1 if char room number is invalid */
int rootonum(char *roo)
{
	int num;
	if (roo[3]=='\0' && roo[0]>48 && roo[0]<55 && ((roo[1]==48 && roo[2]>48 && roo[2]<58) | (roo[1]==49 && roo[2]==48)))
	{
		if (roo[1]==48)
		num=(roo[0]-49)*10+roo[2]-49;
		else
			num=(roo[0]-49)*10+9;
		return num;
	}
	return -1;
}

/**
 * @brief       Determines whether the specified year is leap year.
 * @param[in]   year  The year
 * @return      True if the specified year is leap year, False otherwise. */
int isLeapYear(int year)
{
	return ((year%4==0 && year%100!=0) || year%400==0);
}

/**
 * @brief       Gets the days form 0000 year 1.1
 * @param[in]   year   The year
 * @param[in]   month  The month
 * @param[in]   day    The day
 * @return      The total days. */
int getDays(int year, int month, int day)
{
	int result = 0;
	int i;
	int m[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
	if(isLeapYear(year))
		m[2]++;
	for(i = 1;i < year;i++)
	{
		result += 365;
		if(isLeapYear(i))
			result ++;
	}
	for(i = 1;i < month;i++)
	{
		result += m[i];
	}
	result += day;
	return result;
}

/**
 * @brief       Determines if a date valid.
 * @param[in]   year   The year
 * @param[in]   month  The month
 * @param[in]   day    The day
 * @return      True if valid, False otherwise. */
int isValid(int year, int month, int day)
{
	if (year<1970)
		return 0;
	if (((month==1) | (month==3) | (month==5) | (month==7) | (month==8) | (month==10) | (month==12)) && ((day>0)&&(day<32)))
		return 1;
	if (((month==4) | (month==6) | (month==9) | (month==11)) && ((day>0)&&(day<31)))
		return 1;
	if (month==2 && isLeapYear(year) && ((day>0)&&(day<30)))
		return 1;
	if (month==2 && !isLeapYear(year) && ((day>0)&&(day<29)))
		return 1;
	return 0;

}

/**
 * @brief       Determines if a date expired.(Whether earlier than today)
 * @param[in]   year   The year
 * @param[in]   month  The month
 * @param[in]   day    The day
 * @return      True if expired, False otherwise. */
int isExpired(int year, int month, int day)
{
	int days_now,days;
	time_t now ;
    struct tm *tm_now ;
    time(&now) ;
    tm_now = localtime(&now) ;
    days_now=getDays(tm_now->tm_year+1900,tm_now->tm_mon+1,tm_now->tm_mday);
    days=getDays(year, month, day);
    if (days_now>days)
    	return 1;
    return 0;
}

/**
 * @brief       Determine whether two booking is conflict
 * @param[in]   i     booking No.1
 * @param[in]   o     booking No.2
 * @return      True is conflict, false otherwise */
int isconfli(struct booking i, struct booking o)
{
	int idays,odays;
	idays=getDays(i.year,i.month,i.day);
	odays=getDays(o.year,o.month,o.day);
	if ((idays>=odays+o.period) | (odays>=idays+i.period))
		return 0;
	else
		return 1;
}

/**
 * @brief       Determine whether the file exists
 * @param[in]   fname  The filename
 * @return      true if exists, false otherwise */
int fexists(const char *fname)
{
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

/** @brief       Calculate the total booked rooms after a offset of delta from
 *              today
 * @param       delta  offset
 * @return      the total numbner of booked rooms */
int isBooked(int delta)
{
	FILE *file;
	struct booking i;
	char dir[14],acc[9];
	int cumcus,j,sum;
	int days_now,days;
	time_t now ;
    struct tm *tm_now ;
    sum=0;
	file=fopen("data/main","rb");
	fread(&cumcus,sizeof(int),1,file);
	fclose(file);
    time(&now) ;
    tm_now = localtime(&now) ;
    days_now=getDays(tm_now->tm_year+1900,tm_now->tm_mon+1,tm_now->tm_mday);
	for (j=1;j<cumcus+1;j++)
	{
		sprintf(dir, "data/%s", numtoacc(acc,j));
		if (fexists(dir))
		{
			file=fopen(dir,"rb");
			while (fread(&i, sizeof(struct booking),1,file)==1)
			{
				 days=getDays(i.year, i.month, i.day);
				if(days_now+delta>=days && days_now+delta<days+i.period)
					sum++;
			}
		}	
	}
	return sum;
}

/** @brief       Just drawing
 * @return      void */
void ui()
{
 printf("       __  __      __       __   _____            __               \n");
 printf("      / / / /___  / /____  / /  / ___/__  _______/ /____  ____ ___ \n");
 printf("     / /_/ / __ \\/ __/ _ \\/ /   \\__ \\/ / / / ___/ __/ _ \\/ __ `__ \\\n");
 printf("    / __  / /_/ / /_/  __/ /   ___/ / /_/ (__  ) /_/  __/ / / / / /\n");
 printf("   /_/ /_/\\____/\\__/\\___/_/   /____/\\__, /____/\\__/\\___/_/ /_/ /_/ \n");
 printf("                                   /____/                          \n");
 printf("--------------------------------------------------------------------\n\n");
 }

/** @brief       For password input and valildation judgement
 * @param       pw    The password
 * @return      just 1 */
int getpw(char* pw)
{
	int up_check,low_check,num_check,i,er;
	char tmp;

	while(1)
	{

		up_check=0;
		low_check=0;
		num_check=0;
		i=0;
		er=0;

	    while(1)
	    {
	    	/**
	    	 * using getchar() to avoid invalid input and String overflow
	    	 */
	    	tmp=getchar();
	    	if (tmp==10)
	        {
	        	/**
	        	 * if input is ENTER, then whatever quit the loop
	        	 */
	            if (!er && i && i!=17 && up_check && low_check && num_check)
	            {   
	            	/**
	            	 * when the array is full, stop recording and turn error on
	            	 */
	                pw[i]='\0';
	                break;
	            }
	            else
	            {
	            	system("cls");     
					ui();
					er=1;
	            	printf("(Must contain uppercase letters, lowercase letters and numbers, the maximum length is 16)\n");
	                printf("Wrong password format! Retry: ");  /* for the corresponding error */
	                break;
	                return 0;
	            }
	        } 
	        if (!er && (i!=17))
	        {
	        	/**
	        	 * check whether there is Upper && Lower char && number
	        	 */
	            if ((tmp>47 && tmp<58)|(tmp>64) && (tmp<91)|(tmp>96) && (tmp<123))
	            {
	                pw[i]=tmp;
	                i++;
	                if (tmp>64 && tmp<91)
	                	up_check=1;
	                else if (tmp>47 && tmp<58)
	                	num_check=1;
	                else
	                	low_check=1;
	            }
	        else er=1;
	        }
	    }
	    if (er)
	    	continue;
	    
	    /* re-Enter */
	    i=0;
		er=0;

		system("cls");
		ui();
		printf("Please re-enter the password: ");
		/**
		 * confirm password
		 */
	    while(1)
	    {
	    	tmp=getchar();
	    	if (!er)
	    	{
	    		if (tmp!=pw[i] && tmp!=10)
	    			er=1;
	    		i++;
	    	}
	    	if (tmp==10 && !er && pw[i-1]=='\0')
	    	{
				return 0;
	    	}
	    	if (tmp==10)
	    	{
	    		system("cls");
	    		ui();
	    		er=1;
				printf("(Must contain uppercase letters, lowercase letters and numbers, the maximum length is 16)\n");
				printf("Does not match! Please retry setting password: ");
	            fflush(stdin);
	    		break;
	    	}
	    }
	}
}

/** @brief       try to make a reservation and test whether there is still room
@param       ibook  book for test
@return      serial room number if successfully booked, -1 for false */
int book(struct booking ibook)
{
	FILE *file;
	struct booking tbook;
	int cumcus,i,j;
	int ava[60];
	char dir[14],acc[9];
	/**
	 * if the room is available, turn the corresponding value in ava array on
	 */
	file=fopen("data/main","rb");
	fread(&cumcus, sizeof(int), 1, file);
	fseek(file, sizeof(int)*3, SEEK_CUR);
	for (i=0;i<60;i++)
	{
		fread(&ava[i],sizeof(int),1,file);
		if (ava[i]!=ibook.room)
			ava[i]=0;
	}
	fclose(file);

	for (i=1;i<cumcus+1;i++)
	{
		sprintf(dir, "data/%s", numtoacc(acc,i));
		if (fexists(dir))
		{
			file=fopen(dir,"rb");
			while (fread(&tbook, sizeof(struct booking),1,file)==1)
			{
				if (isconfli(tbook,ibook))
				{
					ava[tbook.room]=0;
				}				
			}
			fclose(file);
		}
	}

	for (i=0;i<60;i++)
	{
		if (!ava[i])
			j=1;
	}
	if (!j)
		return -1;
	/**
	 * randomly choose one room in all available rooms
	 * 
	 * using i to loop from one to sixty
	 * 
	 * using j to control the Randomness
	 */
	cumcus=1+rand()%59;
	i=0;
	j=0;
	while(j<cumcus)
	{
		if (ava[i])
			j++;
		i++;
		if (i==60)
			i=0;
	}
	return i;
}

/** @brief       delete a single booking by create a temp file
 * @param       acc   target account -i the serial number of booking to be
 *                    deleted
 * @param       i     count
 * @return      void */                              
void delete(char *acc, int i)
{
	char dir[14];
	int j;
	struct booking copy;
	FILE *orig;
	FILE *temp;
	/**
	 * create temporary file for the delete operation
	 */
	sprintf(dir, "data/%s", acc);
	orig=fopen(dir,"rb");
	temp=fopen("data/temp","wb");
	j=0;
	while(fread(&copy, sizeof(struct booking), 1, orig))
	{
		j++;
		if (j!=i)
			fwrite(&copy, sizeof(struct booking),1, temp);
	}
	fclose(temp);
	fclose(orig);
	temp=fopen("data/temp","rb");
	orig=fopen(dir,"wb");

	while(fread(&copy, sizeof(struct booking),1,temp))
	{
		fwrite(&copy, sizeof(struct booking),1,orig);
	}
	fclose(temp);
	fclose(orig);
	remove("data/temp");
	/**
	 * delete the temporary file
	 */
}

/**
 * @brief       the customer menu and a few other functions
 * @param       acc   The targeted account
 * @return      for quit */
int customer(char* acc)
{
	int i,er,tmp;
	int room[60];
	float money,change;
	float m[3];
	FILE *file;
	char roo[4],dir[14];
	struct booking tbook;
	sprintf(dir, "data/%s", acc);
	er=0;
	while(1)
	{
		system("cls");
		ui();
		file=fopen(dir, "rb");
		tmp=0;
		i=0;
		/**
		 * Print the bookings list
		 */
		printf("bookings:\n");
		printf("   |   Room    |       Date       |  Period  |   Payment\n");
		while (fread(&tbook, sizeof(struct booking),1,file))
		{
			i++;
			printf("%2d |", i);
			printf("    %s    |    %2d/%2d/%d    |    %2d    |    ",numtoroo(roo, tbook.room) , tbook.month, tbook.day, tbook.year, tbook.period);
			if (tbook.paid)
				printf("Paid\n");
			else
				printf("Unpaid\n");
		}
		fclose(file);
		if (!i)
			printf("-------------------------Empty!--------------------------\n");
		printf("\n");

		/**
		 * menu
		 */
		if (er==1)
			printf("Invalid input! Please Retry...\n\n");
		er=0;
		tmp=0;
		printf("Choose from the following options:\n");
	    printf("\n");
	    printf("        1.Add booking\n");
	    printf("        2.Delete booking\n");
	    printf("        3.Check-out\n");
	    printf("        4.Back to previous page\n");
	    printf("\n");
	    printf("Input your choice and press Enter: ");
	    if (!scanf("%d",&tmp) | ((tmp!=1) && (tmp!=2) && (tmp!=3) && (tmp!=4)))
	    {
	    	fflush(stdin);
	    	er=1;
	    	continue;
	    }
	    fflush(stdin);
	    /**
	     * create a booking
	     * 
	     * at the same time check whether the input booking is valid
	     */
	    if (tmp==1)
	    {
	    	tbook.paid=0;
	    	do
	    	{
	    		system("cls");
	    		ui();
	    		if (er)
	    			printf("Invalid input! Please Retry...\n\n");
	    		er=0;
	    		printf("Please enter the room class (2=** 3=*** 4=****): ");
	    		if (!scanf("%d",&tbook.room) | ((tbook.room!=2) && (tbook.room!=3) && (tbook.room!=4)))
	    			er=1;
	    		fflush(stdin);
	    	} while (er);
	    	
	    	do
	    	{
	    		system("cls");
	    		ui();
	    		if (er)
	    			printf("Invalid input! Please Retry...\n\n");
	    		er=0;
	    		printf("Please enter your Check-in date(month/day/year): ");
	    		if (!(scanf("%d/%d/%d",&tbook.month,&tbook.day,&tbook.year)==3 && isValid(tbook.year,tbook.month,tbook.day) && !isExpired(tbook.year,tbook.month,tbook.day)))
	    			er=1;
	    		fflush(stdin);
	    	} while (er);

	    	do
	    	{
	    		system("cls");
	    		ui();
	    		if (er)
	    			printf("Invalid input! Please Retry...\n\n");
	    		er=0;
	    		printf("Please enter your period of stay (day): ");
	    		if (!(scanf("%d", &tbook.period) && tbook.period>0))
	    			er=1;
	    		fflush(stdin);
	    	} while(er);
	    	
	    	/**
	    	 * Save the booking or remind that no room available
	    	 */
	    	tbook.room=book(tbook);
	    	if (tbook.room!=-1)
	    	{
	    		system("cls");
	    		ui();
	    		printf("You have successfully booked room %s in %d/%d/%d for %d days!",numtoroo(roo,tbook.room),tbook.month,tbook.day,tbook.year,tbook.period);
	    		file=fopen(dir, "a+b");
	    		fwrite(&tbook, sizeof(struct booking),1,file);
	    		fclose(file);
	    		printf("\n\nPress Enter to go back to previous page...");
	    		fflush(stdin);
	    		getchar();
	    		fflush(stdin);
	    		continue;
	    	}
	    	else
	    	{
	    		system("cls");
	    		ui();    		
	    		printf("No room available!\n\nPress Enter to go back to previous page...");
	    		fflush(stdin);
	    		getchar();
	    		fflush(stdin);
	    		continue;
	    	}
	    }

	    if (tmp==2)
	    {
	    	er=0;
	    	/**
	    	 * list bookings for delete
	    	 */
	    	while(1)
	    	{
		    	system("cls");
		    	ui();
		    	file=fopen(dir,"rb");
			    i=0;
			    printf("Bookings:\n");
			    printf("   |   Room    |       Date       |  Period  |   Payment\n");
				while (fread(&tbook, sizeof(struct booking),1,file))
				{
					i++;
					printf("%2d |", i);
					printf("    %s    |    %2d/%2d/%d    |    %2d    |    ",numtoroo(roo, tbook.room) , tbook.month, tbook.day, tbook.year, tbook.period);
					if (tbook.paid)
						printf("Paid\n");
					else
						printf("Unpaid\n");
				}
				fclose(file);
				if (!i)
				{
					printf("-------------------------Empty!--------------------------\n\nPress Enter to go back to previous page...");
					fflush(stdin);
					getchar();
					fflush(stdin);
					break;
				}
				else
				{
					printf("\n");	
				
					
					if (er)
						printf("Invalid input! Please retry...\n\n");
					er=0;
					printf("Please enter the serial number of the booking to be deleted (0 to back to previous page): ");
					if (!(scanf("%d",&tmp)==1 && tmp>-1 && tmp<i+1))
					{
						er=1;
						fflush(stdin);
						continue;
					}
			
					fflush(stdin);
					if (!tmp)
						break;
					else
					{
						tmp=0;
						printf("Are you sure to delete the booking(Yes=1 No=Any other keys): ");
						scanf("%d",&tmp);
						fflush(stdin);
						if (tmp==1)
						{
							delete(acc,tmp);
							printf("Successfully deleted a booking!\n\nPress Enter to go back to countinue...");
							fflush(stdin);
							getchar();
							fflush(stdin);
						}
						continue;
					}
				}
			}
			continue;
	    }

	    if (tmp==3)
	    {
	    	file=fopen("data/main","rb");
	    	fseek(file,sizeof(int),SEEK_SET);
	    	fread(&m[0],sizeof(float),1,file);
	    	fread(&m[1],sizeof(float),1,file);
	    	fread(&m[2],sizeof(float),1,file);
	    	for (i=0;i<60;i++)
	    	{
	    		fread(&room[i],sizeof(int),1,file);
	    	}
	    	fseek(file,sizeof(char)*34,SEEK_CUR);
	    	fread(&money,sizeof(float),1,file);
	    	fclose(file);
	    	system("cls");
	    	ui();
			tmp=0;
			er=0;
			i=0;
			change=0;
			/**
			 * list the bookings and calculate changes
			 */
			printf("Bookings:\n");
			printf("   |   Room    |       Date       |  Period  |   Payment\n");
			file=fopen(dir,"rb");
			while (fread(&tbook, sizeof(struct booking),1,file))
			{
				change+= (float) (1- tbook.paid)*m[room[tbook.room]-2]*tbook.period;
				i++;
				printf("%2d |", i);
				printf("    %s    |    %2d/%2d/%d    |    %2d    |    ",numtoroo(roo, tbook.room) , tbook.month, tbook.day, tbook.year, tbook.period);
				if (tbook.paid)
					printf("Paid\n");
				else
					printf("Unpaid\n");
			}
			fclose(file);
			if (!i)
				printf("-------------------------Empty!--------------------------\n");
			printf("\n");

			printf("Total...................................................%.2lf\n",change);
					tmp=0;
			printf("Whether pay or not? (Yes=1 No=Any other keys): ");
			scanf("%d",&tmp);
			fflush(stdin);
			if (tmp==1)
			{
				file=fopen(dir,"r+b");
				while(fread(&tbook, sizeof(struct booking),1,file))
				{
					tbook.paid=1;
					fseek(file,-24,SEEK_CUR);
					fwrite(&tbook,sizeof(struct booking),1,file);
					fseek(file,0,SEEK_CUR);
				}
				fclose(file);
				/**
				 * Save the cumulative income
				 */
				money+=change;
				file=fopen("data/main","r+b");
				fseek(file,sizeof(int)+sizeof(float)*3+sizeof(int)*60+sizeof(char)*34,SEEK_SET);
				fwrite(&money,sizeof(float),1,file);
				fclose(file);
				printf("\nSuccessfully Paid!\n\nPress Enter to go back to previous page...");
				fflush(stdin);
				getchar();
				fflush(stdin);
			}
			continue;
	    }

	    if (tmp==4)
	    {
	    	fflush(stdin);
	    	return 0;
	    }
		return 0;
	}
}

/** @brief       menu for custumer database management
 * @param[in]   isMan  Indicates if manager
 * @return      for quit */
int database(int isMan)
{
	int er,tmp,i;
	FILE *file;
	char buf;
	char dir[14],acc[9];
	er=0;
	/**
	 * Database menu
	 */
	while(1)
	{
	    	system("cls");
	    	ui();
			if (er==1)
				printf("Invalid input! Please Retry...\n\n");
			er=0;
			tmp=0;
			printf("Choose from the following options:\n");
		    printf("\n");
		    printf("        1.Add Customers\n");
		    printf("        2.Edit Customers (Bookings and Check-out)\n");
		    printf("        3.Delete Customers\n");
		    printf("        4.Back to previous page\n");
		    printf("\n");
		    printf("Input your choice and press Enter: ");
		    if (!scanf("%d",&tmp) | ((tmp!=1) && (tmp!=2) && (tmp!=3) && (tmp!=4)))
		    {
		    	fflush(stdin);
		    	er=1;
		    	continue;
		    }

		    if (tmp==1)
		    {
		    	/**
		    	 * Autometically generate account number and create the new customer account
		    	 */
		    	system("cls");
		    	ui();
		    	file=fopen("data/main", "r+b");
		    	fread(&i, sizeof(int),1, file);
		    	i++;
		    	fseek(file, 0, SEEK_SET);
		    	fwrite(&i, sizeof(int),1,file);
		    	fclose(file);
		    	sprintf(dir, "data/%s", numtoacc(acc,i));
				file=fopen(dir,"wb");
				fclose(file);
		    	printf("Successfully added new customer!");
		    	printf("Your customer number is %s", acc);
		    	printf("\n\nPress Enter to edit it...");
		    	fflush(stdin);
		    	getchar();
		    	fflush(stdin);
		    	customer(numtoacc(acc,i));
		    	fflush(stdin);
		    	continue;
		    }

		    if (tmp==2)
		    {
		    	er=0;
		    	/**
		    	 * check whether a customer exists.
		    	 * if exists, use custumer() function to edit it
		    	 */
		    	while(1)
		    	{
			    	system("cls");
			    	ui();
			    	i=0;
			    	if (er)
			    		printf("No such customer! Retry...\n\n");
			    	er=0;
			    	printf("Please enter the customer number (eight digits) to be edited (0 to cancel): ");
			    	fflush(stdin);
			    	while(1)
			    	{
			    		buf=getchar();
			    		if (buf==10)
			    			break;
			    		if (i<8)
			    		{
			    			acc[i]=buf;
			    			acc[i+1]='\0';
			    			i++;
			    		}
			    		else
			    		{
			    			er=1;
			    			break;
			    		}
			    	}
			    	if (er)
			    		continue;
			    	
			    	if (acc[0]==48 && acc[1]=='\0')
			    	{
			    		fflush(stdin);
			    		break;
			    	}

			    	sprintf(dir, "data/%s", acc);
			    	if (!fexists(dir) | (dir[5]=='m'))
			    	{
			    		er=1;
			    		continue;
			    	}
			    	else
			    	{
			    		customer(acc);
			    		break;
			    	}
		    	}
		    	continue;
		    }

		    if (tmp==3)
		    {
		    	/**
		    	 * check whether a customer exists and try to delete it
		    	 */
		    	if (!isMan)
		    	{
		    		system("cls");
		    		ui();
		    		printf("You don't have permission to delete customers! Please call for your manager...\n\n");
		    		printf("Press Enter to go back to previous page...");
		    		fflush(stdin);
		    		getchar();
		    		fflush(stdin);
		    		continue;
		    	}

		    	er=0;
		    	while(1)
		    	{
			    	system("cls");
			    	ui();
			    	i=0;
			    	if (er)
			    		printf("No such customer! Retry...\n\n");
			    	er=0;
			    	printf("Please enter the customer number (eight digits) to be deleted (0 to cancel): ");
			    	fflush(stdin);
			    	while(1)
			    	{
			    		buf=getchar();
			    		if (buf==10)
			    			break;
			    		if (i<8)
			    		{
			    			acc[i]=buf;
			    			acc[i+1]='\0';
			    			i++;
			    		}
			    		else
			    		{
			    			er=1;
			    			break;
			    		}
			    	}
			    	if(er)
			    		continue;

			    	if (acc[0]==48 && acc[1]=='\0')
			    	{
			    		fflush(stdin);
			    		break;
			    	}

			    	sprintf(dir, "data/%s", acc);
			    	if (!fexists(dir) | (dir[5]=='m'))
			    	{
			    		er=1;
			    		continue;
			    	}
			    	else
			    	{
			    		remove(dir);
			    		printf("Successfully deleted customer!\n\nPress Enter to go back to previous page...");
			    		fflush(stdin);
			    		getchar();
			    		fflush(stdin);
			    		break;
			    	}
		    	}
		    	continue;
		    }

			if (tmp==4)
			{
				fflush(stdin);
				return 0;
			}

		return 0;
	}
}

/**
 * @brief       manager menu
 * @return      for quit */
int manager()
{
	int er,tmp,i,j;
	int type[60];
	char roo[4],pw[17];
	char buf;
	float money;
	FILE *file;

	/**
	 * all the functions that only available for manager
	 */
	er=0;
	while(1)
	{
		tmp=0;
		system("cls");
		ui();
		if (er==1)
			printf("Invalid input! Please Retry...\n\n");
		er=0;
		tmp=0;
		printf("Choose from the following options:\n");
	    printf("\n");
	    printf("        1.Amend Classes for Rooms\n");
	    printf("        2.Amend Prices for Classes\n");
	    printf("        3.Manage Customer database\n");
	    printf("        4.Hotel Statistics\n");
	    printf("        5.Reset manager or receptionist password\n");
	    printf("        6.Logout\n");
	    printf("\n");
	    printf("Input your choice and press Enter: ");
	    if (!scanf("%d",&tmp) | ((tmp!=1) && (tmp!=2) && (tmp!=3) && (tmp!=4) && (tmp!=5) && (tmp!=6)))
	    {
	    	fflush(stdin);
	    	er=1;
	    	continue;
	    }
	    fflush(stdin);

	    if (tmp==1)
	    {
	    	/**
	    	 * list all the rooms with their classes for changeing
	    	 */
	    	er=0;
	    	while(1)
	    	{
		    	system("cls");
		    	ui();
		    	file=fopen("data/main","rb");
		    	fseek(file,sizeof(int)*4, SEEK_SET);

		    	for (i=0;i<60;i++)
		    	{
		    		fread(&type[i], sizeof(int), 1, file);
		    	}
				fclose(file);
				printf("Rooms:\n");
		    	for (j=0;j<6;j++)
		    	{
		    		for (i=j*10;i<j*10+10;i++)
		    		{
		    			printf("%s ",numtoroo(roo, i));
		    			if (type[i]==2)
		    				printf("**   |");
		    			if (type[i]==3)
		    				printf("***  |");
		    			if (type[i]==4)
		    				printf("**** |");
		    		}
		    		printf("\n");   		
		    	}
		    	if (er)
		    		printf("\nNo such room! Retry...\n");
		    	er=0;
		    	printf("\nPlease enter the number of the room to be changed (0 to cancel): ");
		    	i=0;
		    	while(1)
		    	{
		    		buf=getchar();
		    		if (buf==10)
		    			break;
		    		else if (i<3)
		    		{
		    			roo[i]=buf;
		    			roo[i+1]='\0';
		    		}
		    		i++;
		    	}

		    	if (roo[0]=='0' && roo[1]=='\0')
		    		break;
		    	if ((rootonum(roo)<0)|(rootonum(roo)>59) | (i!=3))
		    	{
		    		er=1;
		    		continue;
		    	}

		    	do{
			    	system("cls");
			    	ui();
			    	if (er)
			    		printf("Invalid input! Please retry...\n");
			    	er=0;
			    	printf("Please enter the type of %s (2 for **, 3 for ***, 4 for ****): ", roo);
			    	if (!scanf("%d",&tmp) | ((tmp!=2) && (tmp!=3) && (tmp!=4)))
				    {
				    	fflush(stdin);
				    	er=1;
				    } 
				}while(er);
			    fflush(stdin);

			    type[rootonum(roo)]=tmp;
			    file=fopen("data/main","r+b");
			    fseek(file, sizeof(int)*4, SEEK_SET);
			    for(i=0;i<60;i++)
					fwrite(&type[i], sizeof(int),1,file);
				fclose(file);
				printf("\nSuccessfully changed! Press any key back to menu...");
				getchar();
				fflush(stdin);
				continue;
			}
			continue;
	    }

	    if (tmp==2)
	    {
	    	/**
	    	 * list prices for three classes for changing
	    	 */
	    	er=0;
	    	while(1)
	    	{
		    	system("cls");
		    	ui();
		    	printf("Prices:\n");
		    	file=fopen("data/main","rb");
		    	fseek(file, sizeof(int), SEEK_SET);
		    	fread(&money, sizeof(float), 1, file);
		    	printf("**   %.2f\n", money);
		    	fread(&money, sizeof(float), 1, file);
		    	printf("***  %.2f\n", money);
		    	fread(&money, sizeof(float), 1, file);
		    	printf("**** %.2f\n\n", money);
		    	fclose(file);
				if (er==1)
					printf("Invalid input! Please Retry...\n\n");
				er=0;
				tmp=0;
				printf("Choose from the following options:\n");
			    printf("\n");
			    printf("        1.Amend Price for Class **\n");
			    printf("        2.Amend Price for Class ***\n");
			    printf("        3.Amend Price for Class ****\n");
			    printf("        4.Back to previous page\n");
			    printf("\n");
			    printf("Input your choice and press Enter: ");
			    if (!scanf("%d",&tmp) | ((tmp!=1) && (tmp!=2) && (tmp!=3) && (tmp!=4)))
			    {
			    	fflush(stdin);
			    	er=1;
			    	continue;
			    }
			    fflush(stdin);

			    er=0;

			    if (tmp==1)
			    {
			    	do
			    	{
			    		system("cls");
			    		ui();
			    		if (er)
			    			printf("Invalid input! Please retry...\n\n");
			    		er=0;
				    	printf("Please enter the price for ** class room: ");
						if (!scanf("%f", &money))
							er=1;
						/**
						 * round to two decimal place
						 */
						money=((int)(money * 100 + .5)/100.0);
						if ((money<0) | (money>1000000))
							er=1;
						fflush(stdin);
					} while (er);
					file=fopen("data/main","r+b");
					fseek(file, sizeof(int), SEEK_SET);
					fwrite(&money, sizeof(float),1,file);
					fclose(file);
			    }

			    if (tmp==2)
			    {
			    	do
			    	{
			    		system("cls");
			    		ui();
			    		if (er)
			    			printf("Invalid input! Please retry...\n\n");
			    		er=0;
				    	printf("Please enter the price for *** class room: ");
						if (!scanf("%f", &money))
							er=1;
						money=((int)(money * 100 + .5)/100.0);
						if ((money<0) | (money>1000000))
							er=1;
						fflush(stdin);
					} while (er);
					file=fopen("data/main","r+b");
					fseek(file, sizeof(int)*2, SEEK_SET);
					fwrite(&money, sizeof(float),1,file);
					fclose(file);
			    }

			    if (tmp==3)
			    {
			    	do
			    	{
			    		system("cls");
			    		ui();
			    		if (er)
			    			printf("Invalid input! Please retry...\n\n");
			    		er=0;
				    	printf("Please enter the price for **** class room: ");
						if (!scanf("%f", &money))
							er=1;
						money=((int)(money * 100 + .5)/100.0);
						if ((money<0) | (money>1000000))
							er=1;
						fflush(stdin);
					} while (er);
					file=fopen("data/main","r+b");
					fseek(file, sizeof(int)*3, SEEK_SET);
					fwrite(&money, sizeof(float),1,file);
					fclose(file);
			    }


			    if (tmp==4)
			    {	
			    	fclose(file);
			    	break;
			    }
			    continue;
			}
			continue;
	    }

	    if (tmp==3)
	    {
	    	/**
	    	 * enter the customer database with manager identity
	    	 */
	    	database(1);
	    	continue;
	    }

	    if (tmp==4)
	    {
	    	/**
	    	 * list importent business statistics
	    	 */
	    	system("cls");
	    	ui();
	    	file=fopen("data/main","rb");
	    	fseek(file,sizeof(int)*61+sizeof(float)*3+sizeof(char)*34,SEEK_SET);
	    	fread(&money,sizeof(float),1,file);
	    	printf("The Cumulative Income is %.2f\n",money);
	    	printf("Today's occupancy rate is: %d/60\n", isBooked(0));
	    	printf("Tomorrow's occupancy rate is: %d/60\n", isBooked(1));
	    	printf("the day after tomorrow's occupancy rate is: %d/60\n\n", isBooked(2));
	    	printf("Press Enter to go back to previous page...");
	    	fflush(stdin);
	    	getchar();
	    	fflush(stdin);
	    	continue;
	    }
	    
	    if (tmp==5)
	    {
	    	/**
	    	 * menu for reset passwords
	    	 */
	    	er=0;
	    	while(1)
	    	{
		    	system("cls");
		    	ui();
		    	if (er==1)
					printf("Invalid input! Please Retry...\n\n");
				er=0;
				tmp=0;
				printf("Choose from the following options:\n");
			    printf("\n");
			    printf("        1.Reset manager password\n");
			    printf("        2.Reset receptionist password\n");
			    printf("        3.Back to previous page\n");
			    printf("\n");
			    printf("Input your choice and press Enter: ");
			    if (!scanf("%d",&tmp) | ((tmp!=1) && (tmp!=2) && (tmp!=3)))
			    {
			    	fflush(stdin);
			    	er=1;
			    	continue;
			    }
			    fflush(stdin);

			    if (tmp==1)
			    {
			    	system("cls");
			    	ui();
			    	printf("(Must contain uppercase letters, lowercase letters and numbers, the maximum length is 16)\n");
					printf("please set manager account password: ");
			    	getpw(pw);
			    	system("cls");
			    	ui();
			    	file=fopen("data/main","r+b");
			    	fseek(file, sizeof(int)*61+sizeof(float)*3,SEEK_SET);
			    	fwrite(pw,sizeof(char)*17, 1, file);
			    	fclose(file);
			    	printf("Successfully changed manager password!\n\nPress any key to countinue...");
					getchar();
					fflush(stdin);
					continue;
			    }

			    if (tmp==2)
				{
					system("cls");
					ui();
					printf("(Must contain uppercase letters, lowercase letters and numbers, the maximum length is 16)\n");
					printf("please set receptionist account password: ");
					getpw(pw);
					system("cls");
			    	file=fopen("data/main","r+b");
			    	fseek(file, sizeof(int)*61+sizeof(float)*3+sizeof(char)*17,SEEK_SET);
			    	fwrite(pw,sizeof(char)*17, 1, file);
			    	fclose(file);
			    	system("cls");
			    	ui();
			    	printf("Successfully changed receptionist password!\n\nPress any key to countinue...");
					getchar();
					fflush(stdin);
					continue;
			    }

			    if (tmp==3)
			    	break;
			}
			continue;
	    }

	    if (tmp==6)
	    {
	    	return 0;
	    } 
	    
	    return 0;
	}
}

int main()
{
	int tmp,i,er,quit;
	float money[4];
	FILE *file;
	int type[60];
	char manager_pw[17],receptionist_pw[17];
	char buf;

	srand((unsigned)time(NULL));
	/**
	 * reseed the random number
	 */
	er=0;
	while(1)
	{
		quit=0;
		system("cls");
		ui();
		if (er==1)
			printf("Invalid input! Please Retry...\n\n");
		er=0;
		tmp=0;
		/**
		 * identity choosing menu
		 */
		printf("Please choose your identity:\n");
	    printf("\n");
	    printf("        1.Maneger\n");
	    printf("        2.Receptionist\n");
	    printf("        3.Quit\n");
	    printf("\n");
	    printf("Input your choice and press Enter: ");
	    if (!scanf("%d",&tmp) | ((tmp!=1) && (tmp!=2) && (tmp!=3)))
	    {
	    	fflush(stdin);
	    	er=1;
	    	continue;
	    }
	    fflush(stdin);

	    if (tmp==1)
	    {
	    	if (fexists("data/main"))
	    	{
	    		file=fopen("data/main", "rb");
	    		fseek(file, sizeof(int)*61+sizeof(float)*3, SEEK_SET);
	    		fread(manager_pw, sizeof(char)*17, 1, file);
	    		fclose(file);
	    		/**
	    		 * manager login process
	    		 */
	    		while(1)
	    		{
		    		system("cls");
		    		ui();
		    		if (er)
		    			printf("Wrong password! Please Retry...\n");
		    		er=0;
		    		i=0;
		    		printf("Please enter manager account password: ");

		 			while(1)
		 			{
		 				buf=getchar();
		 				if (!er)
		    			{
		    				if (buf!=manager_pw[i] && buf!=10)
		    					er=1;
		    				i++;
		    			}
				    	if (buf==10 && !er && manager_pw[i-1]=='\0')
				    	{
				    		printf("\n");
				    		printf("Login successfully, press Enter to countinue...");
				    		getchar();
				    		fflush(stdin);
				    		manager();
				    		fflush(stdin);
				    		quit=1;
				    		break;
				    	}
				    	if (buf==10)
				    	{
				    		er=1;
				    		break;
				    	}		 			
		 			}
		 			if(quit)
		 				break;
		 			continue;
		 		}
		 		continue;
	    	}
			
			else
			{
				/**
				 * for the very first initialization (creating files and sn on)
				 */
				system("cls");
				ui();

				printf("This is the first time you open this software!\n\n");
				printf("(Must contain uppercase letters, lowercase letters and numbers, the maximum length is 16)\n");
				printf("please set manager account password: ");
				getpw(manager_pw);
				system("cls");
				ui();
				printf("(Must contain uppercase letters, lowercase letters and numbers, the maximum length is 16)\n");
				printf("please set receptionist account password: ");
				getpw(receptionist_pw);
				system("cls");
				ui();
				
				er=0;

				/**
				 * setting the prices
				 */
				do
				{
				system("cls");
				ui();
				if (er)
					printf("Invalid input! Please retry...\n\n");
				er=0;
				printf("(CNY & Accurate to two decimal places & Less than 1 million)\n");
				printf("Please enter the price for ** class room: ");
				if (scanf("%f", &money[0])!=1)
					er=1;
				fflush(stdin);
				} while (er);
				/**
				 * round to two decimal place
				 */
				money[0]=((int)(money[0] * 100 + .5)/100.0);
				
				do
				{
				system("cls");
				ui();
				if (er)
					printf("Invalid input! Please retry...\n\n");
				er=0;
				printf("(CNY & Accurate to two decimal places & Less than 1 million)\n");
				printf("Please enter the price for *** class room: ");
				if (scanf("%f", &money[1])!=1)
					er=1;
				fflush(stdin);
				} while (er);
				money[1]=((int)(money[1] * 100 + .5)/100.0);
								
				do
				{
				system("cls");
				ui();
				if (er)
					printf("Invalid input! Please retry...\n\n");
				er=0;
				printf("(CNY & Accurate to two decimal places & Less than 1 million)\n");
				printf("Please enter the price for **** class room: ");
				if (scanf("%f", &money[2])!=1)
					er=1;
				fflush(stdin);
				} while (er);
				money[2]=((int)(money[2] * 100 + .5)/100.0);
				/**
				 * save all the settings
				 */
				system("mkdir .\\data");
				file=fopen("data/main", "wb");
				tmp=0;
				fwrite(&tmp, sizeof(int), 1, file);
				fwrite(&money[0], sizeof(float),1,file);
				fwrite(&money[1], sizeof(float),1,file);
				fwrite(&money[2], sizeof(float),1,file);
				for(i=0;i<20;i++)
					type[i]=2;
				for(i=20;i<40;i++)
					type[i]=3;
				for(i=40;i<60;i++)
					type[i]=4;
				for(i=0;i<60;i++)
					fwrite(&type[i], sizeof(int),1,file);
				fwrite(manager_pw, sizeof(char)*17, 1, file);
				fwrite(receptionist_pw, sizeof(char)*17,1,file);
				money[3]=0;
				fwrite(&money[3],sizeof(float),1,file);
				fclose(file);
				/**
				 * list all the initial setting for necessary record
				 */
				system("cls");
				ui();
				printf("Your Setting:\n\n");
				printf("		Maneger Password: %s\n",manager_pw);
				printf("		Receptionist Password: %s\n",receptionist_pw);
				printf("		Price for Class **: %.2f\n",money[0]);
				printf("		Price for Class ***: %.2f\n",money[1]);
				printf("		Price for Class ****: %.2f\n",money[2]);
				printf("		Default setting of room Classes: **101~210 ***301~410 ****501~610\n\n");
				/**
				 * ASCII ART again! I love drawing!
				 */
				printf("      ___      _                ___                _     _       _ \n");
				printf("     / __| ___| |_ _  _ _ __   / __|___ _ __  _ __| |___| |_ ___| |\n");
				printf("     \\__ \\/ -_)  _| || | '_ \\ | (__/ _ \\ '  \\| '_ \\ / -_)  _/ -_)_|\n");
				printf("     |___/\\___|\\__|\\_,_| .__/  \\___\\___/_|_|_| .__/_\\___|\\__\\___(_)\n");
				printf("                       |_|                   |_|                   \n");

				printf("\nPress Enter to login to manager account...");
				fflush(stdin);
				getchar();
				fflush(stdin);
				manager();
				fflush(stdin);
				continue;
			}
	    }

	    if (tmp==2)
	    {
	    	if (fexists("data/main"))
	    	{
	    		file=fopen("data/main", "rb");
	    		fseek(file, sizeof(int)*61+sizeof(float)*3+sizeof(char)*17, SEEK_SET);
	    		fread(receptionist_pw, sizeof(char)*17, 1, file);
	    		fclose(file);
	    		er=0;
	    		/**
	    		 * receptionist login process
	    		 */
	    		while(1)
	    		{
		    		system("cls");
					ui();
		    		if (er)
		    			printf("Wrong password! Please Retry...\n");
		    		er=0;
		    		i=0;
		    		printf("Please enter receptionist account password: ");

		 			while(1)
		 			{
		 				buf=getchar();
		 				if (!er)
		    			{
		    				if (buf!=receptionist_pw[i] && buf!=10)
		    					er=1;
		    				i++;
		    			}
				    	if (buf==10 && !er && receptionist_pw[i-1]=='\0')
				    	{
				    		printf("\n");
				    		printf("Login successfully, press Enter to countinue...");
				    		getchar();
				    		fflush(stdin);
				    		database(0);
				    		fflush(stdin);
				    		quit=1;
				    		break;
				    	}
				    	if (buf==10)
				    	{
				    		er=1;
				    		break;
				    	}		 			
		 			}
		 			if (quit)
		 				break;
		 			continue;
		 		}
		 		continue;
	    	}
	    	else
	    	{
	    		/**
	    		 * receptionist cannot do the initialization for the system
	    		 */
	    		system("cls");
	    		ui();
	    		printf("This is the first time you open this software!\nplease call for your manager...");
	    		printf("\n\nPress Enter to countinue...");
	    		fflush(stdin);
	    		getchar();
	    		fflush(stdin);
	    		continue;
	    	}
	    }

	    if (tmp==3)
	    {
	    	/**
	    	 * just quit
	    	 */
	    	return 0;
	    }
	    return 0;
	}
}
