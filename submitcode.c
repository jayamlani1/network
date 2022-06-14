/*
 * The answers are matching correctly. But the spaces are the problem because of which the 
 * test cases are failing.
 * I was looking at RFC https://datatracker.ietf.org/doc/html/rfc1035 . Which does not 
 * seem to have AAAA record type. So I have added that in default case.
 */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>

//Globals have the first character capital vs local variable which are all small case.

char header[255];
int Opcode;
int Rcode;
int QR;
int AA;
int TC;
int RD;
int RA;

int Flags[16];
int Id;
int Query;
int Answer;
int AuthorityRecord;
int AdditionalRecord;

char Opcode_value[3][8] = {"QUERY\0", "IQUERY\0", "STATUS\0"};

char Enabled_flag[32] = "";

char Rcode_value[6][32] = {"NOERROR\0", "FORMATERROR\0", "SERVERFAILURE\0", "NAMEERROR\0", "NOTIMPLEMENTED\0", "REFUSED\0"};
char List_type[17][7] = {"", "A\0", "NS\0", "MD\0", "MF\0", "CNAME\0", "SOA\0", "MB\0", "MG\0", "MR\0", "NULL\0", "WKS\0", "PTR\0", "HINFO\0", "MINFO\0", "MX\0", "TXT\0"};
char List_class[5][3] = {"", "IN\0", "CS\0", "CH\0", "HS\0"};
int Ip_address[4];
char Qname[255];
int Qname_len;
int Count = 24; 

int Lengthofsite;
int Lengthofdomain;

int Number_question;
int Number_answer;
int Number_authority_record;
int Number_additional_record;

int HextoDec(char *hex_value, int len)
{
    int dec_value = 0;
    int base = 1;


    for(int i = len-1; i>=0; i--)
    {
        if(hex_value[i] >= '0' && hex_value[i]<= '9')
        {
            dec_value = dec_value + (hex_value[i] - 48) * base;
            base = base * 16;
        }
        else if(hex_value[i] >= 'A' && hex_value[i]<= 'F')
        {
            dec_value = dec_value + (hex_value[i] - 55) * base;
            base = base * 16;
        }
        else if(hex_value[i] >= 'a' && hex_value[i]<= 'f')
        {
            dec_value = dec_value + (hex_value[i] - 87) * base;
            base = base * 16;
        }
    }
    return dec_value;
}

void HextoAscii(char *word, int len, char *hex_value)
{
    int letter;
    int k=0;
    /*
     * This code converts the hex value to ascii string and stores it in the character string sent as parameter to the function.
    */
    for(int i=0;i<(len*2) ;i=i+2)
    {
        letter = HextoDec(hex_value + i, 2);
        word[k] = (char) letter;
        k++;
    }
    word[k] = '\0'; 
        
}
void HextoBin(char *hex_value, int len, int *bin_value)
{
    /*
     * This function converts hex to binary value, useful for flags.
    */ 
    for(int i=0;i<len;i++)
    {
        switch(hex_value[i])
        {
            case '0':
                        bin_value[i*4] = 0;
                        bin_value[(i*4) + 1] = 0;
                        bin_value[(i*4) + 2] = 0;
                        bin_value[(i*4) + 3] = 0;
                break;
            case '1':
                        bin_value[i*4] = 0;
                        bin_value[(i*4) + 1] = 0;
                        bin_value[(i*4) + 2] = 0;
                        bin_value[(i*4) + 3] = 1;
                        break;
            case '2':
                        bin_value[i*4] = 0;
                        bin_value[(i*4) + 1] = 0;
                        bin_value[(i*4) + 2] = 1;
                        bin_value[(i*4) + 3] = 0;
                        break;
            case '3':
                        bin_value[i*4] = 0;
                        bin_value[(i*4) + 1] = 0;
                        bin_value[(i*4) + 2] = 1;
                        bin_value[(i*4) + 3] = 1;
                        break;
            case '4':
                        bin_value[i*4] = 0;
                        bin_value[(i*4) + 1] = 1;
                        bin_value[(i*4) + 2] = 0;
                        bin_value[(i*4) + 3] = 0;
                        break;
            case '5':
                        bin_value[i*4] = 0;
                        bin_value[(i*4) + 1] = 1;
                        bin_value[(i*4) + 2] = 0;
                        bin_value[(i*4) + 3] = 1;
                        break;
            case '6':
                        bin_value[i*4] = 0;
                        bin_value[(i*4) + 1] = 1;
                        bin_value[(i*4) + 2] = 1;
                        bin_value[(i*4) + 3] = 0;
                        break;
            case '7':
                        bin_value[i*4] = 0;
                        bin_value[(i*4) + 1] = 1;
                        bin_value[(i*4) + 2] = 1;
                        bin_value[(i*4) + 3] = 1;
                        break;
            case '8':
                        bin_value[i*4] = 1;
                        bin_value[(i*4) + 1] = 0;
                        bin_value[(i*4) + 2] = 0;
                        bin_value[(i*4) + 3] = 0;
                        break;
            case '9':
                        bin_value[i*4] = 1;
                        bin_value[(i*4) + 1] = 0;
                        bin_value[(i*4) + 2] = 0;
                        bin_value[(i*4) + 3] = 1;
                        break;
            case 'a':
                        bin_value[i*4] = 1;
                        bin_value[(i*4) + 1] = 0;
                        bin_value[(i*4) + 2] = 1;
                        bin_value[(i*4) + 3] = 0;
                        break;
            case 'b':
                        bin_value[i*4] = 1;
                        bin_value[(i*4) + 1] = 0;
                        bin_value[(i*4) + 2] = 1;
                        bin_value[(i*4) + 3] = 1;
                        break;
            case 'c':
                        bin_value[i*4] = 1;
                        bin_value[(i*4) + 1] = 1;
                        bin_value[(i*4) + 2] = 0;
                        bin_value[(i*4) + 3] = 0;
                        break;
            case 'd':
                        bin_value[i*4] = 1;
                        bin_value[(i*4) + 1] = 1;
                        bin_value[(i*4) + 2] = 0;
                        bin_value[(i*4) + 3] = 1;
                        break;
            case 'e':
                        bin_value[i*4] = 1;
                        bin_value[(i*4) + 1] = 1;
                        bin_value[(i*4) + 2] = 1;
                        bin_value[(i*4) + 3] = 0;
                        break;
            case 'f':
                        bin_value[i*4] = 1;
                        bin_value[(i*4) + 1] = 1;
                        bin_value[(i*4) + 2] = 1;
                        bin_value[(i*4) + 3] = 1;
                        break;
        }
    }

}

void BuildHeader()
{
    /*
     * This function does the standard print of header section.
     */
     
    printf(";; ->>HEADER<<- opcode: %s, status: %s, id: %d\n", Opcode_value[Opcode], Rcode_value[Rcode] , Id);

    printf(";; flags:%s; QUERY: %d, ANSWER: %d, AUTHORITY: %d, ADDITIONAL: %d\n", Enabled_flag, Number_question, Number_answer, Number_authority_record, Number_additional_record);
    printf("\n");
}

void BuildQuestion()
{
    /*
     * This function creates and prints the question section of the DNS message.
     */

    Count = 24;
    printf(";; QUESTION SECTION:\n");

    int len = HextoDec(header + Count, 2);
    Count = Count + 2;
    char name[255];
    Qname_len = 0;
    while( len != 0 )
    {
        
        HextoAscii(name , len, header + Count);
        Count = Count + (len*2);     
        strcat(Qname, name);
        strcat(Qname, ".");
        Qname_len = Qname_len + len + 1;
        len = HextoDec(header + Count, 2);
        Count = Count + 2;
    }
    
    int Qtype_value = HextoDec(header + Count, 4);
    Count = Count + 4;
    int Qclass_value = HextoDec(header + Count, 4);
    Count = Count + 4;
    char Qtype[7];
    if(Qtype_value > 16)
    {
        switch(Qtype_value)
        {
            case 252:
                        strcpy(Qtype, "AXFR");
                        break;
            case 253:
                        strcpy(Qtype, "MAILB");
                        break;
            case 254:
                        strcpy(Qtype, "MAILA");
                        break;
            case 255:
                        strcpy(Qtype, "*");
                        break;
            default:
                        strcpy(Qtype, "AAAA");
                        break;
        }
    }
    else
    {
        strcpy(Qtype, List_type[Qtype_value]);
    }
    

    char Qclass[7];
    if(Qclass_value == 255)
    {
        strcpy(Qclass, "*");                        
    }
    else 
    {                       
        strcpy(Qclass, List_class[Qclass_value]);
    }
                
    printf(";%s         %s      %s\n", Qname,Qclass,Qtype);

    printf("\n");

}

int BuildName(char *Aname, int lcount)
{
    int len =0;

    /* 
     * This is a recursive function, which is used to create the name for answer section.
     * I wrote the question section first, and then realised there are some complecated scenario. That should be taken care of.
     * Hence wrote this function.
     */
        
    if(header[lcount] == 'c')
    {
            
        lcount++;
        int TCount = HextoDec(header + lcount, 3);
        Count = Count + 4;
        len = BuildName(Aname, TCount *2 );
        if(len == 0)
        return 0;
    }
    else
    {
            
        len = HextoDec(header + lcount, 2);
        lcount = lcount+2;
        char name[255];
        while( len != 0)
        {
            HextoAscii(name , len, header + lcount);
            lcount = lcount + (len*2);
            strcat(Aname, name);
            strcat(Aname, ".");


            if(header[lcount] == 'c')
            {
                lcount++;
                int TCount = HextoDec(header + lcount, 3);
                len = BuildName(Aname, TCount*2);
            }
            else
            {
                len = HextoDec(header + lcount, 2);
                lcount = lcount + 2;
            }
        }
    }
    return 0;
}

void BuildAnswer()
{
    
    
    /*
     * This function creates and prints the answer section.
     */
    printf(";; ANSWER SECTION:\n");

    int len;
    for(int i =0;i< Number_answer;i++)
    {
        char Aname[255];
        Aname[0] = '\0';
        len = BuildName(Aname, Count);

        int Atype_value = HextoDec(header + Count, 4);
        Count = Count + 4;
        int Aclass_value = HextoDec(header + Count, 4);
        Count = Count + 4;
        char Atype[7];
        if(Atype_value > 16)
        {
            switch(Atype_value)
            {
                case 252:
                            strcpy(Atype, "AXFR");
                            break;
                case 253:
                            strcpy(Atype, "MAILB");
                            break;
                case 254:
                            strcpy(Atype, "MAILA");
                            break;
                case 255:
                            strcpy(Atype, "*");
                            break;
                default:
                            strcpy(Atype, "AAAA");
                            break;
            }
        }
        else
        {
                strcpy(Atype, List_type[Atype_value]);
        }


        char Aclass[7];
        if(Aclass_value == 255)
        {
                strcpy(Aclass, "*");
        }    
        else
        {
                strcpy(Aclass, List_class[Aclass_value]);
        }

        Count = Count +4;
        int TTL = HextoDec(header + Count, 4);
        Count = Count +4;
        
        if(Atype_value == 5)
        {
            /*
             * This condition handles the case where the type is Cname.
             */
            char Cname[255];
            Cname[0] = '\0';
            int p1 = HextoDec(header + Count, 4);
            Count = Count + 4 ;
            len = BuildName(Cname, Count);
            Count = Count + (p1*2);
            printf("%s        %d    %s    %s    %s\n", Aname, TTL, Aclass, Atype, Cname);
        }
        else if(Atype_value == 1)
        {
            /* 
             * Thia condition handles ipv4
             */
            Count = Count+4;    
            int p1 = HextoDec(header + Count, 2);
            Count = Count + 2;
            int p2 = HextoDec(header + Count, 2);
            Count = Count + 2;
            int p3 = HextoDec(header + Count, 2);
            Count = Count + 2;
            int p4 = HextoDec(header + Count, 2);
            Count = Count + 2;
            printf("%s        %d    %s    %s    %d.%d.%d.%d\n", Aname, TTL, Aclass, Atype, p1, p2, p3, p4);
        }
        else
        {
            /*
             * This condition is for IPV6
             */

            Count = Count+4;
            int prev = 0;
            int k =0;
            char ipv6[40];
            for(int j=0;j<8;j++)
            {
                int temp = HextoDec(header + Count, 4);
                if(temp == 0)
                {
                    if(prev == 0)
                    {
                        prev = 1;
                        ipv6[k] = ':';
                        k++;
                    }
                    Count = Count + 4;    
                }
                else
                {
                    for(int z = 0;z<4;z++)
                    {
                        ipv6[k+z] = header[Count+z];
                    }
                    k = k + 4;
                    Count = Count + 4;
                    if(j != 7)
                    {
                        ipv6[k] = ':';
                        k++;
                    }
                }
            }
            ipv6[k] = '\0';
            printf("%s        %d    %s    %s    %s\n", Aname, TTL, Aclass, Atype, ipv6);    
        }
                    
    }    
    Count = Count + 1;

}    

void makeflags()
{
    QR = Flags[0];
    AA = Flags[5];
    TC = Flags[6];
    RD = Flags[7];
    RA = Flags[8];

    // Opcode can have either 0, 1 or 2 rest of them are kept for future use.

    Opcode = (Flags[4] * 1) + (Flags[3] * 2);         
    //Rcode can have 0 to 5 values rest are kept for future use.

    Rcode = (Flags[15] * 1) + (Flags[14] * 2) + (Flags[13] * 4);

    Number_question = HextoDec(header + 8, 4);
    
    Number_answer = HextoDec(header + 12, 4);
    
    Number_authority_record = HextoDec(header + 16, 4);

    Number_additional_record = HextoDec(header + 20, 4);


    if(QR == 1)
    {
        strcat(Enabled_flag, " qr");
    }
    if(AA == 1)
    {
        strcat(Enabled_flag, " aa");
    }
    if(TC == 1)
        {
                strcat(Enabled_flag, " tc");
        }
        if(RD == 1)
        {
                strcat(Enabled_flag, " rd");
        }
    if(RA == 1)
        {
                strcat(Enabled_flag, " ra");
        }
    
}

int main() {

    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
   scanf("%s", header);

   // Scan the data here, after that build the header section, question section and answer section.

   // Getting the ID from the message
   Id = HextoDec(header, 4);

   // Getting the flags from the message
   HextoBin(header+4, 4, Flags);

    /* 
     * These are the main set of function, which will first create each seciion and print the same.
     */
   makeflags(); 
   BuildHeader();  
   BuildQuestion();
   BuildAnswer();  
    return 0;
}
