#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	long long result=(long long)a*(long long)b;
	return (FLOAT)(result>>16);
}


FLOAT F_div_F(FLOAT a, FLOAT b) {
	/* Dividing two 64-bit integers needs the support of another library
	 * `libgcc', other than newlib. It is a dirty work to port `libgcc'
	 * to NEMU. In fact, it is unnecessary to perform a "64/64" division
	 * here. A "64/32" division is enough.
	 *
	 * To perform a "64/32" division, you can use the x86 instruction
	 * `div' or `idiv' by inline assembly. We provide a template for you
	 * to prevent you from uncessary details.
	 *
	 *     asm volatile ("??? %2" : "=a"(???), "=d"(???) : "r"(???), "a"(???), "d"(???));
	 *
	 * If you want to use the template above, you should fill the "???"
	 * correctly. For more information, please read the i386 manual for
	 * division instructions, and search the Internet about "inline assembly".
	 * It is OK not to use the template above, but you should figure
	 * out another way to perform the division.
	 */

	FLOAT la,lb;
	if(a<0)
		la=-a;
	else
		la=a;
	if(b<0)
		lb=-b;
	else
		lb=b;
	int z=la/lb;
	la=la%lb;
	int i;
	for(i=0;i<16;i++){
		la<<=1;
		z<<=1;
		if(la>=lb){
			la=la-lb;
			z++;
		}
	}
	FLOAT result;
	if((a<0&&b>0)||(a>0&&b<0))
		result=-z;
	else
		result=z;
	return result;


}

FLOAT f2F(float a) {
	/* You should figure out how to convert `a' into FLOAT without
	 * introducing x87 floating point instructions. Else you can
	 * not run this code in NEMU before implementing x87 floating
	 * point instructions, which is contrary to our expectation.
	 *
	 * Hint: The bit representation of `a' is already on the
	 * stack. How do you retrieve it to another variable without
	 * performing arithmetic operations on it directly?
	 */
	FLOAT la=*(FLOAT *)&a;
	FLOAT t=(la&0x007fffff)|0x00800000;
	FLOAT order=(la&0x7f800000)>>23;	
	FLOAT s=order-127;
	if(s<7){
		if(la>0)
			return (t>>(7-s));
		else
			return -(t>>(7-s));
	}
	else{
		if(la>0)
			return (t<<(s-7));
		else
			return -(t<<(s-7));
	}
}

FLOAT Fabs(FLOAT a) {
	if(a<0)
		return -a;
	else
		return a;
}

/* Functions below are already implemented */

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

