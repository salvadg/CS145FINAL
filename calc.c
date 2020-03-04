/* Salvador Gutierrez
	89291450
	06/13/2019 
	
	ISSUE: input > 32,767 (int max) will not work. 
*/	


#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "avr.h"
#include "lcd.h"


/* Array to get char representations of button value */
char keypad[17] = {'1', '2', '3', '+','4', '5', '6', '-','7', '8', '9', 'C','*', '0', '#', '/'};
char buf[16];
char num[16] = {0};
// Stack struct
struct Stack
{
	int capacity;
	int top;
	long *items;
	int len;
	int len2;
	double* data;
};

struct Stack* init_Stack(int capacity)
{
	struct Stack* stack = (struct Stack*) malloc(sizeof(struct Stack));
	stack ->capacity = capacity; // set capacity of stack
	stack ->top = -1;
	stack -> len = 0;
	stack -> len2 = 0;
	
	stack ->items = (long*) malloc(stack ->capacity * sizeof(long));
	stack ->data = (double*) malloc(stack ->capacity * sizeof(double));
	return stack;
}

int isFull(struct Stack* stack)
{
	return stack->top == stack ->capacity-1;
}

int isEmpty(struct Stack* stack)
{
	return stack->top == -1;
}

void push(struct Stack* stack, long val)
{
	if(isFull(stack))
		return;
	stack->items[++stack->top] = val;
	stack->len+= 1;
	
}

void fpush(struct Stack* stack, double val)
{
	// 	if(isFull(stack))
	// 		return;
	stack->data[++stack->top] = val;
	stack->len2 += 1;
	
}

long peek(struct Stack* stack)
{
	if(!isEmpty(stack))
	return stack ->items[stack->top];
	else
	return 0;
}

long pop(struct Stack* stack)
{
	
	if(isEmpty(stack))
		return -1;// cant pop empty stack
	
	stack->len -=1;
	return stack->items[stack->top--];
	
	
}

double fpop(struct Stack* stack)
{
	
	if(isEmpty(stack))
	return -1;// cant pop empty stack
	
	stack->len2 -= 1;
	return stack->data[stack->top--];
	
	
}


// Queue Implementation
long queue[16]; int front = 0; int rear = -1; int qSize = 0;
int MAX = 16;

int qFull()
{
	return qSize == MAX;
}

int qEmpty()
{
	return qSize == 0;
}

void enqueue(long val )
{
	if(!qFull())
	{
		if(rear == MAX-1)
			rear = -1;
		queue[++rear] = val;
		qSize++;
	}

}

long dequeue()
{
	long data = queue[front++];
	if(front == MAX)
	{
		front = 0;
	}
	qSize--;
	return data;
}
long getFront()
{
	if(qEmpty())
	{
		return -1;
	}
	return queue[front];
}
// done queue

void displayQ(int len)
{
	int i;
	printf("Q: ");
	for(i = 0; i < len; i++)
	{
		if(queue[i] > 41 && queue[i] < 48)
		{
			printf("%c ",queue[i]);
			
		}
		else
		printf("%d ",queue[i]);
	}
	
	printf("\n");
	
}

void printStack(struct Stack* s)
{
	int i;
	printf("Stack: ");
	for(i = 0; i < s->len; i++)
	{
		if(s->items[i] > 41 && s->items[i] < 48)
		{
			printf("%c ",s->items[i]);
			
		}
		else
		printf("%c ",s->items[i]);
	}
	
	printf("\n");
}

double postFixCalc()
{
	struct Stack *stack = init_Stack(16);
	double result = 0;
	long op;
	while(qSize!= 0)
	{
		
		op = dequeue();
		if(op > 41 && op < 48)
		{
			
			double rOp = fpop(stack);
			double lOp = fpop(stack);
			//printf("EXP: %.2f %c %.2f\n", lOp,op,rOp);
			
			if((int)op == 42 ) // "*"
			{
				result = lOp*rOp;
			}
			if((int)op == 47 ) // "division"
			{
				result = lOp/rOp;
			}
			if((int)op == 43 ) // add
			{
				result = lOp+rOp;
			}
			if((int)op == 45 ) // subtract
			{
				result = lOp-rOp;
			}
			
			fpush(stack, result);
		}
		else
		{
			fpush(stack, op);
		}
		
	} // end while
	
	return result;
}

int parseIn(int size, char * str)
{
	struct Stack* opStack = init_Stack(16);
	// create queue
	long result;
	char temp[16];
	int i = 0;
	int x = 0;
	while( i < size)
	{
		if((int)str[0] > 41 && (int)str[0] <48)
		{
	
			return 1;
		}

		if(i == (size-1))
		{
			temp[i] ='\0';
			result = atoi(temp);
			memset(temp,0,16); // empty temp array
			// put code here to enqueue number into queue
			enqueue(result);
		}
		if(str[i] == '/' || str[i] == '*')
		{
			
			temp[i] ='\0';
			result = atoi(temp);
			memset(temp,0,16); // empty temp array
			// put code here to enqueue number into queue
			enqueue(result);
			
			if(peek(opStack) == '+' || peek(opStack) == '-')
			{
				//printf("push* %c\n", str[i]);
				push(opStack, str[i]);
			}
			else // top stack has higher/equal precedence
			{
				//pop higher precedence then enqueue
				if(peek(opStack) != 0)
				enqueue( pop(opStack) );
				//printf("push/ %c\n", str[i]);
				push(opStack,str[i]); // push lower precedence
				
			}
			x = 0; // reset temp count
			i++;
			continue;
			
		}
		
		if(str[i] == '-' || str[i] == '+')
		{
			temp[i] ='\0';
			result = atoi(temp);
			memset(temp,0,16); // empty temp array
			// put code here to enqueue number into queue
			enqueue(result);
			
			if(peek(opStack) != 0)
			enqueue( pop(opStack) );
			
			if(peek(opStack) == 45 || peek(opStack) == 43)
			enqueue( pop(opStack) );
			//printf("push+ %c\n", str[i]);
			push(opStack,str[i]); // push lower precedence
			
			x = 0; // reset temp count
			i++;
			continue;
			
		}
		else
		{
			temp[x] = str[i];
			x++;
		}
		i++;
		
	}
	
	//Now we have finished processing push everything
	// in stack to queue
	displayQ(qSize);

	for(i = 0; i <= opStack->len; i++)
	{
		enqueue( pop(opStack));
	}
	displayQ(qSize);
	
	// call postFixCalc
	char s[17];
	sprintf(s,"%.6f",postFixCalc());
	lcd_pos(1,0);
	lcd_puts2(s);

	return 0;
}

int is_pressed(int r,int c)
{
	DDRC = 0xFF; // if DDRC = 0x0F then need to clear rows
	PORTC = 0xFF; // if set to 0xF0 Then need to clear columns
	
	// set row pins to weak 1
	CLR_BIT(DDRC, r); //if DDRC is intially set to 0 then no need to clear bit
	//SET_BIT(PORTC, r);
	
	
	// Set column pins to strong 0
	//SET_BIT(DDRC,c+4);
	CLR_BIT(PORTC, c+4); //if PORTC is intially set to 0 then no need to clear bit

	
	avr_wait(5); // without wait keypad wont detect input properly

	
	if(GET_BIT(PINC, r)) // this will be 1 when unpressed
	{
		return 0;
	}
	return 1;

}

int get_key() {
	// Iterate through rows and cols until we find what key is pressed else return no key pressed (0)
	int row, col;
	for (row = 0; row < 4; ++row) {
		for (col = 0; col < 4; ++col) {
			if (is_pressed(row,col)) {
				return (row * 4 +col)+1 ;
			}
		}
	}
	return 0;
}

int main(void)
{

	avr_init(); // initialize avr for functions
	lcd_init(); // initialize lcd
	
	int released = 1; // to avoid reading input multiple times when its held

// 	char t[16] = {0};
// 		
// 	enqueue(1000000);
// 	enqueue(9);
// 	enqueue('*');
// 	sprintf(t, "%f", postFixCalc());
// 	lcd_puts2(t);
// 	return 1;
	int i = 0;

	while (1)
	{
		
		int key = get_key();
		if(key == 0)
		released = 1;
		
		if(keypad[key-1] == 'C' && released)// if user pushes C clear lcd and buf
		{
			lcd_clr();
			memset(num,0,16);
			released = 0;
			i=0;
		}

		if(keypad[key-1] == '#') // Add
		{
			if(i > 0)
			{
				int x = i+1;
				lcd_clr();
				avr_wait(500);
				lcd_puts2(num);
				parseIn(++i,num);
				if( parseIn(x, num) == 1)
			 	{
			 		lcd_clr();
			 		lcd_pos(0,2);
			 		memset(num,0,16);
			 		lcd_puts2("SYNTAX ERROR!");
			 		continue;
			 	}
				
				
				
				//memset(num,0,16);
				
				//i = 0;

			}
		}
		else
		{
			if(key > 0 && released)
			{
				//lcd_pos(0,i);
				num[i] = keypad[key-1];
				lcd_put(num[i]);
				i += 1;
				released = 0;
			}
		}
		
		avr_wait(100);
		
		
	} // end while

}


