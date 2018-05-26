//2‚Â‚Ì’l‚ÌŠÔ‚Ì‘˜a‚ğ‹‚ß‚é
#include <stdio.h>

void sumof(int a, int b){

	int max;
	int min;
	int sum = 0;

	if (a < b){
		max = b;
		min = a;
		for (min; min <= max; min++){
			sum += min;
		}
	}
	else{
		max = a;
		min = b;
		for (min; min <= max; min++){
			sum += min;
		}
	}
	printf("%d‚Æ%d‚ÌŠÔ‚Ì‘S®”‚Ì˜a‚Í%d‚Å‚·", a, b, sum);
}

int main(void){

	int a, b;

	printf("2‚Â‚Ì’l‚ğ“ü—Í‚µ‚Ä‚­‚¾‚³‚¢\n‚P‚Â–ÚF");
	scanf("\n%d", &a);

	printf("‚Q‚Â–ÚF");
	scanf("\n%d", &b);

	sumof(a, b);

	rewind(stdin);
	getchar();

	return 0;
}