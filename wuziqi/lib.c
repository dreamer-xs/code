
void delay(volatile unsigned int i)
{
	while(--i);
}

void *memset(void *s, int c, unsigned int n)
{
	int i;
	for(i = 0; i < n; i++)
		((char *)s)[i] = c;

	return s;
}

