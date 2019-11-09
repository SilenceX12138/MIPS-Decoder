#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
typedef struct tagRtype{
	char *name;
	int rs;
	int rt;
	int rd;
}Rtype;
typedef struct tagItype{
	char *name;
	int rs;
	int rt;
	int imm;
}Itype;
typedef struct tagJtype{
	char *name;
	int index;
}Jtype;
char *get_slice(char s[],int l,int r);
int get_val(char c);
char *hex_to_bin(char hex_code[]);
int bin_to_dec(char binum[]);
char *get_Rname(char func[]);
char *get_Iname(char op[]);
char *get_Jname(char op[]);
Rtype get_RIns(char hex_code[]);
Itype get_IIns(char hex_code[]);
Jtype get_JIns(char hex_code[]);
int main(void)
{
	char hex_code[101];
	Rtype tmpr;
	Itype tmpi;
	Jtype tmpj;
	freopen("code.txt","r",stdin);
	while(~scanf("%s",hex_code)){
		if(!strcmp("000000",get_slice((hex_to_bin(hex_code)),0,5))){
			tmpr=get_RIns(hex_code);
			if(!strcmp(tmpr.name,"nop"))puts("nop");
			else printf("%-4s %4d %4d %4d\n",tmpr.name,tmpr.rd,tmpr.rs,tmpr.rt);
		}
		else if(get_Iname(get_slice((hex_to_bin(hex_code)),0,5))){
			tmpi=get_IIns(hex_code);
			printf("%-4s %4d %4d %04x\n",tmpi.name,tmpi.rt,tmpi.rs,tmpi.imm);
		}
		else if(get_Jname(get_slice((hex_to_bin(hex_code)),0,5))){
			tmpj=get_JIns(hex_code);
			printf("%-4s %08x\n",tmpj.name,tmpj.index);
		}
	}
	fclose(stdin);
	getch();
	return 0;
}

char *hex_to_bin(char hex_code[])
{
	int i,j;
	int tmpval;
	char *bin_code;
	bin_code=(char *)malloc(sizeof(char)*101);		//如果位数不够填充空字符 结尾会拼接乱码 
	memset(bin_code,0,sizeof(char)*101);
	for(i=0;i<strlen(hex_code);i++){
		tmpval=get_val(hex_code[i]);
		for(j=3;j>=0;j--){
			bin_code[i*4+j]=tmpval%2+'0';
			tmpval/=2;
		}
	}
	return bin_code;
}

int get_val(char c)
{
	if(isdigit(c))return c-'0';
	return tolower(c)-'a'+10;
}

char *get_Rname(char func[])
{
	if(!strcmp(func,"100001"))return "addu";
	if(!strcmp(func,"100011"))return "subu";
	if(!strcmp(func,"101011"))return "sltu";
	if(!strcmp(func,"101010"))return "slt";
	if(!strcmp(func,"100101"))return "or";
	if(!strcmp(func,"001000"))return "jr";
	if(!strcmp(func,"000000"))return "nop";
	return NULL;
}

char *get_Iname(char op[])
{
	if(!strcmp(op,"001101"))return "ori";
	if(!strcmp(op,"001111"))return "lui";
	if(!strcmp(op,"100011"))return "lw";
	if(!strcmp(op,"101011"))return "sw";
	if(!strcmp(op,"000100"))return "beq";
	if(!strcmp(op,"101000"))return "sb";
	if(!strcmp(op,"100000"))return "lb";
	return NULL;
}

char *get_Jname(char op[])
{
	if(!strcmp(op,"000011"))return "jal";
	return NULL;
}

Rtype get_RIns(char hex_code[])
{
	Rtype tmp;
	char *bin_code=hex_to_bin(hex_code);
	memset(&tmp,0,sizeof(Rtype));
	tmp.name=get_Rname(get_slice(bin_code,26,31));
	tmp.rs=bin_to_dec(get_slice(bin_code,6,10));
	tmp.rt=bin_to_dec(get_slice(bin_code,11,15));
	tmp.rd=bin_to_dec(get_slice(bin_code,16,20));
	return tmp;
}

char *get_slice(char s[],int l,int r)
{
	char *tmp;
	tmp=(char *)malloc(sizeof(char)*101);
	int i,j,k;
	k=strlen(s);
	if(l>k||r>k||l>=r||l<0||r<0)return "illegal range";
	for(i=l,j=0;i<=r;i++,j++){
		tmp[j]=s[i];
	}
	tmp[j]='\0';
	return tmp;
}

int bin_to_dec(char binum[])
{
	int val=0;
	int i;
	int k=strlen(binum);
	for(i=0;i<k;i++){
		val+=binum[i]-'0';
		if(i!=k-1)val*=2;
	}
	return val;
}

Itype get_IIns(char hex_code[])
{
	Itype tmp;
	char *bin_code=hex_to_bin(hex_code);
	memset(&tmp,0,sizeof(Itype));
	tmp.name=get_Iname(get_slice(bin_code,0,5));
	tmp.rs=bin_to_dec(get_slice(bin_code,6,10));
	tmp.rt=bin_to_dec(get_slice(bin_code,11,15));
	tmp.imm=bin_to_dec(get_slice(bin_code,16,31));
	return tmp;
}

Jtype get_JIns(char hex_code[])
{
	Jtype tmp;
	char *bin_code=hex_to_bin(hex_code);
	memset(&tmp,0,sizeof(Jtype));
	tmp.name=get_Jname(get_slice(bin_code,0,5));
	tmp.index=bin_to_dec(get_slice(bin_code,6,31));
	return tmp;
}

