#include <string.h>
#include <math.h>


int StrToInt(char *s)
{
	/*int l = strlen(s);
	char c;
	int i = 0, res = 0;
	int sign = 1;
	char zero = '0';

	c = s[i];
	if (c == '-') 
	{
		sign = -1;
		i++;
		c = s[i];
	}

	while (i <= l-1)
	{
		if ((c <= '0') && (c >= '9')) MessageBoxA(0, "c is space!!!", "", 0);
		res += (int)(c - zero)*pow(10.0f, l-i-1); 
		i++;
		c = s[i];
	}
	res *= sign;*/
	return atoi(s);
}

float StrToFloat(char *s)
{
	char intpart[128], floatpart[128];
	int i = 0, j = 0;
	char c;

	float res;

	c = s[i];
	while ((c != '.') && (c!=0))
	{
		intpart[j] = c;
		i++;
		j++;
		c = s[i];
	}
	intpart[j] = 0;

	if (c == 0)
	{
		res = (float)StrToInt(intpart);
		return res;
	}

	i++;
	j = 0;
	c = s[i];
	while (c != 0)
	{
		floatpart[j] = c;
		i++;
		j++;
		c = s[i];
	}
	floatpart[j] = 0;

	res = atoi(intpart);

	int l = strlen(floatpart);
	i = 0;
	char zero = '0';

	c = floatpart[i];
	while (i <= l-1)
	{
		if ((c <= '0') && (c >= '9')) MessageBoxA(0, "c is space!!!", "", 0);
		res += (float)(c-zero)*pow(1/10.0f, (i+1));
		i++;
		c = floatpart[i];
	}

	
	return res;//atof(s);
}


char *strgetword(char *s, char sep, int n)
{
	int i = 0, k = 1;
	char c;
	c = s[i];
	while (c != 0)
	{
		if (c == sep) k++;
		if (k == n) break;
		i++;
		c = s[i];
	}
	i++;
	k = 0;
	char res[64];
	c = s[i];

	if (n == 1)
	{
		res[0] = s[0];
		k = 1;
	}

	while ((c != 0) && (c != sep))
	{
		res[k] = c;
		i++;
		k++;
		c = s[i];
	}
	res[k] = 0;

	return res;
}
void ReadString(FILE *f, char* res)
{
	char c;

	int i = 0;
	fread(&c, 1, 1, f);
	if ((c == 10) || (c == 13)) fread(&c, 1, 1, f);

	while (c == ' ')
	{
		fread(&c, 1, 1, f);
	}

	while ((c != ' ') && (c != 0xd) && (c != 0xa))
	{
		res[i] = c;
		i++;
		fread(&c, 1, 1, f);
	}
	res[i] = 0;
}


void ReadNum(FILE *f, char* res)
{
	char c;

	int i = 0;
	fread(&c, 1, 1, f);
	while (((c >= 48) && (c <= 57) && (c != 0xd)) || (c == '-') || ( c == '.'))
	{
		res[i] = c;
		i++;
		fread(&c, 1, 1, f);
	}
	res[i] = 0;
}