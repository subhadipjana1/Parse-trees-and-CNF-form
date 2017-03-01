#include<stdio.h>      // Declaring all header files
#include<stdlib.h>
#include<string.h>
#include <strings.h>

#define max 1000000
#define ROW 26
#define COLUMN 2
                                       // Color codes for text in the interface
#define ANSI_COLOR_RED     "\x1b[31m"  //red
#define ANSI_COLOR_GREEN   "\x1b[32m"  //green
#define ANSI_COLOR_YELLOW  "\x1b[33m"  //yellow
#define ANSI_COLOR_BLUE    "\x1b[34m"  //blue
#define ANSI_COLOR_MAGENTA "\x1b[35m"  //magenta
#define ANSI_COLOR_CYAN    "\x1b[36m"  //cyan
#define ANSI_COLOR_RESET   "\x1b[0m"   //setting color to default terminal text color

//*********************************************************************DECLARATION OF VARIBLES AND FUNCTIONS*****************************************************************************************

//Varible Declarations
char stack_ini[max];  // stack for finding prefix and infix done using array
char char_array[10000000]; // contains the infix expression entered
int store_arr[59];   // stores truth value of atom in this array based on the ASCII value of atom 
int CNF_History=0;  // check whether CNF expression expression was already calculated or not

//definition of the node structure
struct node {
   char data;       // literal of the node( ~,+,*,>,or any other atom)
   int value;       //truth value
   struct node* rc;  //right node pointer
   struct node* lc;  //left node pointer
};

struct node* stack[1000000];  // stack used for building the tree ,refer to buildtree()
struct node* temp;  // temporary pointer to be used in conversion to prefix
struct node* temp2; 

struct node* root;  // root of original parse tree
struct node* root2; // root of CNF expression
char prefix[1000000];//prefix array
char infix[1000000];//infix array
char prefix2[100000]; // identical to prefix, to keep a record of the original expression that was entered
char truthValue;    // stores truth value of the expression

int store_arr[59];

int top=-1;        // to indicate stack is empty at the beginning
int selection, choice;  // interface variables ,that stores user's choice
long long int n;
int count=0;

//Function Declarations
void infixToPrefix(char infix[1000000],char prefix[1000000]); //storing infix ,prefix

void reverse(char entry[1000000]);//function to reverse the input

char pop();   //removing last entered element from s101000000
void push(char c); //input element to stack

int precedence(char c); // check for precedence of operators to find prefix order

int operatorCheck(char c);  // check that the operator is only among the 6
                            // operators (including parenthesis).

void pushStack(struct node* t); //push the node into the stack(for task 2)

struct node* popStack(); //pop the node from the stack

void buildTree(); // create parse tree

void buildTree2();  // creating the duplicate parse tree

void inOrderTraversal(struct node *a); // inOrderTraversal of parse tree created using build tree

int height(struct node *root);  // height of parse Tree

void create (char *infix, long long int limit);     // generate a random infix expression

void initialise_store_arr(int store_arr[]);  // initialise all elements of the array to value 2

void reset();     // converts to prefix and builds the parse tree

void reset2();  // make a duplicate parse tree for CNF manipulation 

void input_truth(struct node *a);   // inputing valuations of the atoms in the parse tree

int evaluate(struct node *node);    //evaluating the expression  using the truth values , enterred using input_truth function.

int find(int left_val,char operator,int right_val);  // contains truth table of basic natural deduction laws(and,or,implication,negation),a

int choiceOfInput();            //Interface related

void convertToPrefix();         //Interface related

void proceed();                 //Interface related

int menu();	                //Interface related

void getInput(int selection);	//Interface related

void print();              //print the CNF expression

void print_tree(struct node *a);  // input CNF expression to a char array

void IMPL_FREE(struct node *a);   // implication free

struct node* NNF(struct node *a);  // negation normal form

struct node* CNF(struct node *a);  // make Conjuctive Normal Form of the infix expression

struct node* DISTR(struct node *a, struct node *b);  // Distributive function ,used in the CNF function

void validity( long long int i );  // Check whether the infix expression is a tautology

int valid=0,i=0;


//******************************************************************************MAIN FUNCTION************************************************************************************************

int main()
{
srand(getpid()); // seed initial numbers for random number generator function

char_array[0] = '\0';

selection=choiceOfInput();  // switch on interface
getInput(selection);        


while(1)            // interface runs as long the user wants it.
	{
	choice= menu();    // for details regarding these functions ,check the function implementations below
    proceed(choice);
	if(choice==0)
	break;
	}


printf("Thank you for visiting\n");
printf("See you soon\n");
return 0;
}


//**************************************************************************END OF MAIN FUNCTION*******************************************************************************************

//*********************************************************************************TASK 1**************************************************************************************************
void convertToPrefix(){

	infixToPrefix(infix,prefix); // Conversion of infix-
	reverse(prefix);             //- statement to prefix statement.
}

void reset(){

  convertToPrefix();   // make prefix and build tree using the prefix expression
  buildTree();

}

void reset2(){         // build a copy of the original tree built using reset() :see above
	
	reverse(prefix2);  
    buildTree2();
   
}

void infixToPrefix(char infix[101000000], char prefix[101000000]){
    int i=0,j=0;
    char token;
    stack_ini[++top]='#';  
    reverse(infix);

    for(i=0;i<strlen(infix);i++){    //Traverse the reversed infix expression and obtain a character at a time 
        token=infix[i];
        if(operatorCheck(token)==0){  //if not operator
            prefix[j]=token;
            j++;
        } else{                      //if it is one of the 6 operators
            if(token==')'){          //  if character is ")" ,push it into stack_ini
                push(token);
            }
            else if(token=='('){     // if character is "(" ,keep on popping stack_ini contents into the 
                while (stack_ini[top]!=')'){  // prefix array until the top of the stack is ")" character and then pop ")" out of the stack_ini 
                    prefix[j]=pop();
                    j++;
                }
                pop();
            }
            else{                       // operator precedence check if infix expression not well paranthesized
                if(precedence(stack_ini[top])<=precedence(token)){
                    push(token);
                }
                else{
                    while (precedence(stack_ini[top])>=precedence(token)){
                        prefix[j]=pop();
                        j++;
                    }
                    push(token);
                }
            }
        }
    }

    while (stack_ini[top]!='#'){
        prefix[j]=pop();        // Till the the time the stack_ini is not empty,popping its contents into prefix array
        j++;

    }
    prefix[j]='\0';
    strcpy(prefix2,prefix);   // copy prefix1 to prefix2
}


void reverse(char entry[1000000]){  // to reverse the expression generated by infixtoPrefix() to get the final prefix expression

    char temp[1000000];        // temporary array to store the reverse expression
    int i,j;

    for(i=strlen(entry)-1,j=0;i>=0;i--,j++){  // iterate along the expression in reverse order and store in temp array
        temp[j]=entry[i];
    }

    temp[j]='\0';                             // indicate end of string by make the last element null character
    strcpy(entry,temp);                       // copy temp to entry array
}

char pop(){            // stack expression to remove the top element in the stack, and return the removed element

    char c;        
    c=stack_ini[top];   // char c is the top element of the stack to be removed
    top--;             // reduce the size of the stzck by one ,as one element has been removed
    return c;
}


void push(char c){     // input an element (char c) to the top of the stack

    top++;             // increase size of the stack by one
    stack_ini[top]=c;  // store the element to the top of the stack
}


int precedence(char c){

    switch (c){        //checking for precedence, higher the return value lower the precedence : called by infixtoPrefix()
        case '~':
            return 5;   
            break;
        case '*':
            return 4;
            break;
        case '+':
            return 3;
            break;
        case '>':
            return 2;
            break;
        case '(':
        case ')':
            return 1;
            break;
        case '#':
            return 0;
            break;
    }
}


int operatorCheck(char c){   // to check whether the node data is an operator or paranthesis, or an atom: function is called by infixtoPrefix()

    switch (c){
        case '~':case '*':case '+':case '>':case '(': case ')':  //if operator return 1 else 0
                return 1;
                break;
        default:
                return 0;
    }
}





//*************************************************************************************TASK 2************************************************************************************************

void pushStack(struct node* t){ //input element to the top of the stack ,and increase the stack size,uses in buildTree ()
    stack[++top]=t;
}


struct node* popStack(){  // return the top element of the stack and reduce the stack size,used in buildTree()
    return (stack[top--]);
}

void buildTree(){           //build the tree using prefix expression
    int i=strlen(prefix)-1;

    while (i>=0){           // traverse along the prefix expression

        switch (prefix[i]){
            case '+':                          
            case '*':                    //case for +,*,> 
            case '>':
                temp=(struct node*)malloc(sizeof(struct node));  // create a node with data as prefix[i] 
                temp->data=prefix[i];
                temp->lc=popStack();      // assign left node
                temp->rc=popStack();      // assign right node 
                pushStack(temp);
                break;

            case '~':                     //case for ~
                temp=(struct node*)malloc(sizeof(struct node));
                temp->data=prefix[i];
                temp->lc=NULL;            //left child is null
                temp->rc=popStack();
                pushStack(temp);
                break;

            default:                      //case for non-operators
                temp=(struct node*)malloc(sizeof(struct node));
                temp->data=prefix[i];
                temp->lc=NULL;            // left child is null
                temp->rc=NULL;            // right child is null
                pushStack(temp);

        }
        i--;
    }
    root=stack[top];                     //storing top of stack in root
}

//*****************************************************//ASSIGNMENT 2//************************************************************************************************

void buildTree2(){  // Build a duplicate tree but with root as root2
	top=-1;         
    int i=strlen(prefix2)-1;

    while (i>=0){                   // traverse along the prefix expression

        switch (prefix2[i]){
            case '+':
            case '*':
            case '>':                                                //case for +,*,>
                temp=(struct node*)malloc(sizeof(struct node));     // create a node with data as prefix[i] 
                temp->data=prefix2[i];
                temp->lc=popStack();                  // assign left node
                temp->rc=popStack();                  // assign right node
                pushStack(temp);
                break;

            case '~':                                  //case for ~
                temp=(struct node*)malloc(sizeof(struct node));
                temp->data=prefix2[i];
                temp->lc=NULL;                        //left child is null
                temp->rc=popStack();                  //assign right node
                pushStack(temp);
                break;

            default:                                  //case for non-operators
                temp=(struct node*)malloc(sizeof(struct node));
                temp->data=prefix2[i];
                temp->lc=NULL;                        // left child is null
                temp->rc=NULL;                        // right child is null
                pushStack(temp);

        }
        i--;
    }
    root2=stack[top];                                //storing top of stack in root
}

//*************************************************************************************TASK 3*************************************************************************************************

void inOrderTraversal(struct node *a) {            // print the infix expression
    if (a == NULL) return;

    printf("(");

    inOrderTraversal(a->lc);                      // traverse fully along left till we reach an atom
    printf("%c",a->data);  
    inOrderTraversal(a->rc);                      // traverse fully along right till we reach an atom 

    printf(")");

}

//*************************************************************************************TASK 4**************************************************************************************************

int height(struct node *h){         // implementation of the height function

  if(h==NULL){return 0;}

  else {                               //calculate height of left subtree and right subtree separately
       int left = height(h->lc);
       int right = height(h->rc);

       if(left>right){                 // choose max of height of(left subtree ,right subtree)

                     return(left+1);
		     }

       else {
            return (right+1);
            }

   }
}


//*************************************************************************************TASK 5**************************************************************************************************

void initialise_store_arr(int store_arr[])   // initialise truth values of all possible leaves of the parse tree to 2 indicating that no truth value has been enterred
                                       
	{
	     int i;

             for(i=0;i<59;i++)
	     {

                   store_arr[i]=2;
             }
	}


void input_truth(struct node *a) // user enters inputs for the atoms in the expression, we use inorder traversal
{

         if (a == NULL) return;

         input_truth(a->lc);

         if( (a->lc==NULL && a->rc==NULL))  // if node 'a' is an atom then only ask for userinput
		{

                       int x,i;
                       char input;

                       i=a->data;
                       i=i-64;            // index of the corresponding leaf in the array according to ASCII value

	               if(store_arr[i]==2)
				{
			        printf("%c : ",a->data);
                 		scanf(" %c",&input);      // prompt user input
                   		x = (input=='T')?1:0;
                  		store_arr[i]=x;         // store truth value at the corresponding in the store_array
				}

       		      else{
    			  x=store_arr[i];             // if the leaf had already been encountered ,do not prompt user input
			  }

         	      a->value=x;                 // assign truth value to respective nodes

 		 }

         input_truth(a->rc);

}


void random_input(struct node *a)      // instead of user entering truth values ,truth value 0 or 1 is randomly assigned to every atom
{                                      // works exactly the same way as user_input()

         if (a == NULL) return;

         random_input(a->lc);

         if( (a->lc==NULL && a->rc==NULL)){
		
            int x = (rand() % 2);      // randomly choose truth value of the leaf
            int i;
            char ch = (x==1)?'T':'F';

            i=a->data;
            i=i-64;

                if(store_arr[i]==2){

                      store_arr[i]=x;                    // store truth value at the corresponding in the store_array
                      printf("%c  :  %c\n",a->data,ch);
                }

                else{
                      x=store_arr[i];
                }

                a->value=x;
    }
                random_input(a->rc);
}



int evaluate(struct node *node)         // find the truth value of expression based upon the inputs
{

     if(node->lc!=NULL || node->rc!=NULL ){     // condition for the node to be a leaf or negation
	  
          int left_val;

          if(node->lc!=NULL){
		
		left_val = evaluate(node->lc);          // traverse along the left of the tree 
		}
		

          else{                                // if node is a negation
              left_val = 0;
              }
              

          int right_val = evaluate(node->rc); // traverse right of tree ,and store truth value of right immediate nde in right_value

          int val = find(left_val,node->data,right_val);     // evaluate sub expression using the find function

          return val;
    	  }

     else {                          // if node is an operator other than negation
          return node->value;        
     	  }
}


int find(int left_val,char operator,int right_val){    // truth value using bitwise operators

             if(operator == '~'){              //negation bitwise operator
               return ((~right_val)+2);
             }

             else if(operator == '*' ){        //and operator  
               return left_val & right_val;
             }

             else if(operator == '+'){        //or operator
               return left_val | right_val;
             }

             else if(operator == '>'){        // a>b = ~a+b,implication operator
               return ((~left_val) | (right_val));
             }
}



void print_tree(struct node *a) {   // store the CNF expression in a character array
    if (a == NULL) return;

                                    // we use inorder traversal of the CNF tree
    char l[2]="(";                 
    char r[2]=")";
    char operator[2];               
    operator[0]=a->data; 
    operator[1]='\0';
    strcat(char_array, l);
    print_tree(a->lc);
    strcat(char_array,operator);
    print_tree(a->rc);
    strcat(char_array,r);

}

void print(){                //print the CNF expression
	
  if(CNF_History==0){
	  print_tree(root2);    // Check if the character array has already been made
	  }
	  
  printf("(");                                             // print the CNF expression in the right format
  for(long long int i=0;char_array[i]!='\0';i++){          // traverse along the char_array string the CNF expression
	  
	  
	  
	  if(char_array[i]!='(' && char_array[i]!='*' && char_array[i]!=')'){   // neglect unnecessary brackets
		  
		  printf("%c",char_array[i]);
		  
		  }
		  
	  else if(char_array[i]=='*'){
		  
		  printf(") * (");
		  }
	  		  	  
}
 printf(")\n");
 CNF_History = 1;        // indicator that the CNF expression has been made
}


void IMPL_FREE(struct node *a) {     // Make the expression implication free

    if (a == NULL) return;     
    IMPL_FREE(a->lc);               // we use inorder traversal, left traversal

    if(a->data == '>'){             // a->b = ~a+b  ie  change root '->' to '+', make left node of '+' as '~', right node as 'b'

    a->data='+';
    struct node *b = a->lc;

    temp=(struct node*)malloc(sizeof(struct node));
    temp->data = '~';
    temp->lc=NULL;
    temp->rc=b;
    a->lc=temp;
	}

    IMPL_FREE(a->rc);     //right traversal
}


struct node* NNF(struct node *a){            // Make the expression in negation normal form after IMPL_FREE has been executed

  if((a->lc == NULL && a->rc == NULL) ||((a->lc == NULL) && (a->rc->data != '~') && (a->rc->data != '+') && (a->rc->data != '*') )){
    return a;                                // check whether expression is a literal ie of the form  'a' or '~a'
  }

  if(a->data == '~' && a->rc->data == '~'){  // replace '~~a' as 'a'
    return NNF(a->rc->rc);
  }

  if(a->data=='*'){                         // NNF(a*b) = NNF(a)*NNF(b)
        struct node* temp10;

   temp10=(struct node*)malloc(sizeof(struct node));
  temp10->data = '*';
  temp10->lc = NNF(a->lc);
  temp10->rc = NNF(a->rc);

  return temp10;    //return node '*'
  }

  if(a->data=='+'){
    struct node* temp11;                   // NNF(a+b) = NNF(a) + NNF(b)

  temp11=(struct node*)malloc(sizeof(struct node));
  temp11->data = '+';
  temp11->lc = NNF(a->lc);
  temp11->rc = NNF(a->rc);

  return temp11;    // return node '+'
  }

  if(a->data == '~' && a->rc->data == '*'){  // node is '~' and right node is 'and' ,then apply de- Morgan's law
                                              //  ~(a*b) = ~a + ~b 
    struct node* temp12;
    struct node* temp13;
    struct node* temp14;

       temp12=(struct node*)malloc(sizeof(struct node));
       temp12->data = '+';                   // create a node '+'

       temp13=(struct node*)malloc(sizeof(struct node));
       temp14=(struct node*)malloc(sizeof(struct node));
       temp13->data = '~';                   // create a node '~' to be the left node of node '+' just created
       temp14->data = '~';                   // create a node '~' to be the right node of node '+' just created

       temp13->lc = NULL;                    //left node of negation is null 
       temp13->rc = a->rc->lc;               //right node of left neagation

       temp14->lc = NULL;                     //left node negation is null
       temp14->rc = a->rc->rc;                //right node of right negation

       temp12->lc = NNF(temp13);
       temp12->rc = NNF(temp14);

       return temp12;

  }

   if(a->data == '~' && a->rc->data == '+'){    // node is '~' and right node is 'or' ,then apply de- Morgan's law
                                              //  ~(a+b) = ~a * ~b 
     struct node* temp15;
     struct node* temp16;
     struct node* temp17;


        temp15=(struct node*)malloc(sizeof(struct node));
        temp15->data = '*';                            // create a node '*'

        temp16=(struct node*)malloc(sizeof(struct node));
        temp17=(struct node*)malloc(sizeof(struct node));
        temp16->data = '~';                       // create a node '~' to be the left node of node '*' just created
        temp17->data = '~';                       // create a node '~' to be the right node of node '*' just created


        temp16->lc = NULL;                         //left node of negation is null 
        temp16->rc = (a->rc->lc);                  //right node of left neagation

        temp17->lc = NULL;                         //left node negation is null 
        temp17->rc = a->rc->rc;                    //right node of right negation 

        temp15->lc = NNF(temp16);                  
        temp15->rc = NNF(temp17);

        return temp15;

   }


}


struct node* CNF(struct node *a){    //Find the CNF of the expression


  if(a->lc==NULL && a->rc==NULL){    // node is a leaf
          return a;}

  if(a->data == '*'){                // CNF(a*b) = CNF(a) * CNF(b)
      struct node* temp4;

    temp4=(struct node*)malloc(sizeof(struct node));
    temp4->data = '*';
    temp4->lc = CNF(a->lc);
    temp4->rc = CNF(a->rc);
    return temp4;
  }

  if(a->data == '+'){               //CNF(a+b) = CNF(a) + CNF(b)
    return DISTR( CNF(a->lc) , CNF(a->rc) );

  }
  if(a->data=='~'){                // if literal is of type ~a
     struct node* temp7;

    temp7=(struct node*)malloc(sizeof(struct node));
    temp7->data = '~';
    temp7->rc = CNF(a->rc);
    return temp7;
  }

}

struct node* DISTR(struct node *a,struct node *b){  //DISTR function

   if(a->data == '*'){     //  DISTR( (a+b),c) ) = DISTR(a*c) + DISTR(b*c)
     struct node* temp5;

     temp5=(struct node*)malloc(sizeof(struct node));     // create node *
     temp5->data = '*';                              
     temp5->lc = DISTR(a->lc,b);
     temp5->rc = DISTR(a->rc,b);
     return temp5;
   }

    if(b->data == '*'){     //  DISTR(a,(b+c)) = DISTR(a*b) + DISTR(a*c)
      struct node* temp6;

      temp6=(struct node*)malloc(sizeof(struct node));   // create node *
      temp6->data = '*';
      temp6->lc = DISTR(a,b->lc);
      temp6->rc = DISTR(a,b->rc);
      return temp6;
    }

    if(a->data != '*' && b->data != '*'){   // DISTR(a,b) = a+b;
       struct node* temp3;

      temp3=(struct node*)malloc(sizeof(struct node));
      temp3->data = '+';
      temp3->lc = a;
      temp3->rc = b;
      return temp3;
    }

}


void validity( long long int i ){  // check whether the given expresssion is valid utilising the CNF expression created

 valid=0;                          // initialise validity to false
 int array [ROW][COLUMN];
 int k;
 
 for(k=0;k<ROW;k++){   // initialising the array to 0
  
  array [k][0]=0;
  array [k][1]=0;
  
  }
  
 int j=0;

 while(char_array[i]!='*'){  // traverse till the next 'and' and count (0 or 1) the no of literals and their negations

    if(char_array[i]=='\0')  //break if null character is encountered
     break;

    if(char_array[i]>='a'&&char_array[i]<='z'){  // if character is encountered
   

    if(char_array[i-1]=='~'||char_array[i-2]=='~'){  // check for a negation infront of it
    
    array[(int)char_array[i]-97][1]=1;
    }
    
   else array[(int)char_array[i]-97][0]=1;

   }
  i++;
  
 }

 for(j=0; j<26 ;j++){  // check for a pair (ex: p , ~p)
  
  if(array[j][0]==1 && array[j][1]==1)
   valid=1;
   
   }
     
 if (valid!=1) // return as false if pair found
  return;

 if(char_array[i]=='*'){  

  validity(i+1);   // recursive call (goes on till '\0' is encountered)
  }
    
 if(char_array[i]=='\0') // no clause left to check
  return;
  
}
//*******************************************************************************INTERFACING FUNCTIONS******************************************************************************************

int choiceOfInput()   // Custom input or Random input_ Interface function
{
	int c;
	printf(ANSI_COLOR_CYAN"Select the type of input:\n"ANSI_COLOR_RESET ANSI_COLOR_RED "1 "ANSI_COLOR_RESET ANSI_COLOR_YELLOW "Custom                "ANSI_COLOR_RED"2 "ANSI_COLOR_RESET ANSI_COLOR_YELLOW"Random" ANSI_COLOR_RESET "\n\n");
        do
		{
	    printf(ANSI_COLOR_CYAN"Your Input : "ANSI_COLOR_RESET);		
		scanf("%d",&c);
		if(c!= 1 && c!= 2)
		printf(ANSI_COLOR_RED"Invalid selection. Press 1 or 2."ANSI_COLOR_RESET"\n");
		}
	while(c!=1 && c!=2);

	if(c==1||c==2)
	return c;
}

int menu()    // Choice of operations to perform on the parse tree
{
	printf(ANSI_COLOR_GREEN"What do you want to do with the formula??"ANSI_COLOR_RESET"\n\n");
	
	printf(ANSI_COLOR_RED"-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"ANSI_COLOR_RESET"\n");
	
	printf(ANSI_COLOR_RED"1 "ANSI_COLOR_RESET ANSI_COLOR_YELLOW "Print the prefix formula"ANSI_COLOR_RESET"\n");
	
	printf(ANSI_COLOR_RED"2 "ANSI_COLOR_RESET ANSI_COLOR_YELLOW"Print the infix formula from the parse tree"ANSI_COLOR_RESET"\n");
	
	printf(ANSI_COLOR_RED"3 "ANSI_COLOR_RESET ANSI_COLOR_YELLOW"Find the height of the Parse Tree"ANSI_COLOR_RESET"\n");
	
	if(selection==1)printf(ANSI_COLOR_RED"4 "ANSI_COLOR_RESET ANSI_COLOR_YELLOW "Evaluate truth value of the formula"ANSI_COLOR_RESET"\n");
	
    else printf(ANSI_COLOR_RED"4 "ANSI_COLOR_RESET ANSI_COLOR_YELLOW"Evaluate truth value of the formula(Randomly Generated Inputs if random infix expression)"ANSI_COLOR_RESET"\n");
    
    printf(ANSI_COLOR_RED"5 "ANSI_COLOR_RESET ANSI_COLOR_YELLOW"Find the CNF expression"ANSI_COLOR_RESET"\n");
    
    printf(ANSI_COLOR_RED"6 "ANSI_COLOR_RESET ANSI_COLOR_YELLOW "Evaluate Validity of the expression"ANSI_COLOR_RESET"\n");
    
    printf(ANSI_COLOR_RED"7 "ANSI_COLOR_RESET ANSI_COLOR_YELLOW "Compare sizes of the original tree and the CNF tree"ANSI_COLOR_RESET"\n");
    
    printf(ANSI_COLOR_RED"8 "ANSI_COLOR_RESET ANSI_COLOR_YELLOW"Print full "ANSI_COLOR_RESET ANSI_COLOR_RED"REPORT "ANSI_COLOR_RESET ANSI_COLOR_YELLOW"of the parse tree"ANSI_COLOR_RESET"\n");
    
	printf(ANSI_COLOR_RED"9 "ANSI_COLOR_RESET ANSI_COLOR_YELLOW"Input another formula"ANSI_COLOR_RESET"\n");
			
 	printf(ANSI_COLOR_RED"0 "ANSI_COLOR_RESET ANSI_COLOR_YELLOW"EXIT\n"ANSI_COLOR_RESET);
 	
 	printf(ANSI_COLOR_RED"-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"ANSI_COLOR_RESET"\n\n");
 	
	printf(ANSI_COLOR_CYAN"Select any option to proceed : "ANSI_COLOR_RESET);
	    
	scanf("%d",&choice);
	
	printf("\n");
	return choice;
}

void proceed()    // Function calls according to the the choice of operation
{
                    // all underneath appear as the interface for the assignment
	switch(choice)
	{
		case 2: printf(ANSI_COLOR_CYAN"INFIX : "ANSI_COLOR_RESET);
		    inOrderTraversal(root);
			printf("\n");
			printf("\n");
			break;
			
		case 1: printf(ANSI_COLOR_CYAN"PREFIX  :  "ANSI_COLOR_RESET"%s",prefix);
			printf("\n");
			printf("\n");
			break;
			
		case 3: printf(ANSI_COLOR_CYAN"HEIGHT OF PARSE TREE : "ANSI_COLOR_RESET ANSI_COLOR_YELLOW"%d",height(root));
			printf(ANSI_COLOR_RESET"\n\n");
			break;
			
		case 4:
            if(selection==1){
                printf(ANSI_COLOR_CYAN"Enter 'T' for true  and 'F' for false\n\n"ANSI_COLOR_RESET);
                initialise_store_arr(store_arr);

                input_truth(root);

			if(evaluate(root)==1){
        truthValue = 'T';
    }
			else{
        truthValue = 'F';

      }

  }
	         else{
             initialise_store_arr(store_arr);
             random_input(root);

             if(evaluate(root)==1){    // display true as 'T'

               truthValue = 'T';

             }
              else{                    // display true as 'F'

                truthValue = 'F';
              }
  }

			printf("Truth value of expression : %c \n",truthValue);
			printf("\n");
			break;
			
		case 9: selection=choiceOfInput();
            getInput(selection);
            CNF_History=0;           // CNF history to check whether cnf expression was already generated
			break;
		
		case 8: printf(ANSI_COLOR_RED"-------------------------"ANSI_COLOR_MAGENTA"Expression Report"ANSI_COLOR_RED"-----------------------------------------------------------------------------------------------------\n"ANSI_COLOR_RESET);
		        for(i=1;i<=7;i++){
			    choice = i;
			    proceed(choice);
			}
			    printf(ANSI_COLOR_RED"-------------------------"ANSI_COLOR_MAGENTA"End of Report"ANSI_COLOR_RESET ANSI_COLOR_RED"----------------------------------------------------------------------------------------------------------\n\n"ANSI_COLOR_RESET);
		break;	   			
			
        case 5: if(CNF_History == 0){
             IMPL_FREE(root2);
             root2 = NNF(root2);
             root2 = CNF(root2);
             
           }
            printf(ANSI_COLOR_CYAN"CNF  : "ANSI_COLOR_RESET);
            print();
            printf("\n");
       break;
      
        case 6: printf(ANSI_COLOR_CYAN"VALIDITY  :  "ANSI_COLOR_RESET);
             if(CNF_History == 0){
             IMPL_FREE(root2);
             root2 = NNF(root2);
             root2 = CNF(root2);
             print_tree(root2);

             CNF_History=1;
           }

            validity( 0 );

            if(valid==1){
              printf(ANSI_COLOR_YELLOW"True\n"ANSI_COLOR_RESET);
          }

           else{
              printf(ANSI_COLOR_YELLOW"False\n"ANSI_COLOR_RESET);
            }
            printf("\n");
      break;

       case 7:if(CNF_History == 0){
            IMPL_FREE(root2);
            root2 = NNF(root2);
            root2 = CNF(root2);
            print_tree(root2);

            CNF_History=1;
          }
           printf(ANSI_COLOR_CYAN"COMPARE SIZE : \n"ANSI_COLOR_RESET);
           printf("The Height of the original tree is : %d\n",height(root));
           printf("The Height of the CNF tree is : %d\n\n",height(root2));
           
	  case 0: return;
			   break;
			
	default: printf(ANSI_COLOR_RED"Please enter a valid choice\n"ANSI_COLOR_RESET);
			 printf("\n");
	}
}

void getInput(int selection)   // Input Infix expression
{
if(selection==1)
	{
	printf(ANSI_COLOR_CYAN"ENTER AN INFIX STATEMENT : " ANSI_COLOR_RESET);  // infix expression enterred by the user
        scanf("%s",infix);
        printf("\n");
        
        reset();
        reset2();
                  
	}
	
else{
   printf(ANSI_COLOR_CYAN"Enter number of Literals :  "ANSI_COLOR_RESET);   // randomly produce a infix expression by enterring number of literals
   scanf("%lld",&n);
   printf("\n");
   char d;
   int y;
   bzero(infix,1000000);

   create(infix,n);  // generate random infix expression
   reset();
   reset2();
   
	}
}

void create (char *infix, long long int limit)
{
 char l[2]={'(','\0'};          // left bracket string
 char r[2]={')','\0'};          // right bracket string
 char bb[2];                  // operator string
 bb[1]='\0';

 int v=(rand() % 26)+97;    // for getting a random alphabet

 int w=(rand() % 4)+1;      // for getting a random operator
 
 switch(w){
  
  case 1: bb[0]='+';
          break;

  case 2: bb[0]='*';
          break;

  case 3: bb[0]='>';
          break;

  case 4: bb[0]='~';
          break;
  }
  
 long long int left =1, right = limit;
 char aa[2]={(char)(v),'\0'};          // alphabet string

 if(limit==1){                // concatenate random operator and alphabets till no of literals is n (following BNF rules of grammar)
  
  strcat(infix,aa);
  
  }
  
 else{
  
  if(w!=4){   

   left= (rand() % (limit-1))+1;
   right= limit-left;
   strcat(infix,l);
   create(infix,left);
   strcat(infix,r);
   strcat(infix,bb);
   strcat(infix,l);
   create(infix,right);
   strcat(infix,r);
   
   }
  else{
   
   strcat(infix,bb);
   strcat(infix,l);
   create(infix,limit);
   strcat(infix,r);
   
   }
  }
}




//********************************************************************************END OF PROGRAM**************************************************************************************************

