
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>


enum menuOptions { DEFAULT, ENTER_PARAMATERS, ALLOCATE_MEMORY,DEALLOCATE_MEMORY,DEFRAGMENT_MEMORY,QUIT}; // Used for optionMenu
enum strategyType{FIRST_FIT, BEST_FIT};
typedef struct node {
    int ID;
    int startingAddressOfBlock;
    int endingAddressOfBlock;
    struct node*next;
} memoryBlock;


void printOptionMenu (int *optionNum);
void enterParameters(memoryBlock**head, int*pmSize, int *strategyType, int* lastHoleSize);
void printAllocationTable(memoryBlock *head);
void allocateMemory(memoryBlock **head, int *pmSize, int strategyType, int* lastHoleSize);
void dealocateMemory(memoryBlock **head, int *pmSize,int* lastHoleSize);
void defragmentMemory(memoryBlock **head);
void destroy(memoryBlock **head);
bool idExists(memoryBlock * head, int inputID);

int main(int argc, const char * argv[]) {
    
    int optionNum=0, pmSize=0, strategyType = -1, lastHoleSize=0;
    memoryBlock * head= NULL;
   
    while(optionNum != QUIT)
    {
        printOptionMenu(&optionNum);
        
        switch (optionNum) {
            case ENTER_PARAMATERS:
                
                enterParameters(&head, &pmSize, &strategyType,&lastHoleSize);
                break;
                
            case ALLOCATE_MEMORY:
                
                allocateMemory(&head, &pmSize, strategyType,&lastHoleSize);
                printAllocationTable(head);
               
                break;
                
            case DEALLOCATE_MEMORY:
                
                dealocateMemory(&head, &pmSize,&lastHoleSize);
                printAllocationTable(head);
                break;
                
            case DEFRAGMENT_MEMORY:
                
               
                defragmentMemory(&head);
                printAllocationTable(head);
                lastHoleSize = pmSize;
        
                break;
                
            case QUIT:
                printf("Quiting program...\n");
                destroy(&head);
                break;
               
            default:
                break;
        }
    
    }//end switch statement
    
    return 1;
}

//Function purpose to print optionMenu and return the optionNumber given by the user
void printOptionMenu (int *optionNum)
{

  int numRead = 0;        //Used for error checking user input;

  printf
    ("\nProcess creation and destruction\n--------------------------------\n");

  printf
    (" %d)Enter parameters\n %d)Allocate memory for a block\n %d)Deallocate memory for a block\n %d)Defragment memory\n %d)Quit program and free memory \n",
     ENTER_PARAMATERS,  ALLOCATE_MEMORY, DEALLOCATE_MEMORY, DEFRAGMENT_MEMORY, QUIT);
    printf("Enter Selection:");
    numRead = scanf ("%d", &*optionNum);

  while (numRead != 1 || *optionNum <= 0 || *optionNum > QUIT)
    {
      printf ("Invalid input, please enter an option bellow\n");

        printf
          (" %d)Enter parameters\n %d)Allocate memory for a block\n %d)Deallocate memory for a block\n %d)Defragment memory\n %d)Quit program and free memory \n", ENTER_PARAMATERS,  ALLOCATE_MEMORY, DEALLOCATE_MEMORY, DEFRAGMENT_MEMORY, QUIT);
          scanf ("%*[^\n]");
        
         numRead = scanf ("%d", &*optionNum);
    }


}//end printOptionMenu(char * optionNum)


//function purpose to set the physical memory size and the strategy type (first fit or best fit) <=> (0 or 1)
void enterParameters(memoryBlock**head, int*pmSize, int *strategyType, int* lastHoleSize)
{
    int numRead = 0; //used for input error checking
    
    printf("Enter size of physical memory:");
    numRead= scanf("%d",&*pmSize);
    while(numRead !=1 || *pmSize <=0)
    {
        printf("Input Error! Please enter a positive integer greater than zero.\n");
        scanf ("%*[^\n]");
        numRead= scanf("%d",&*pmSize);
    }
    
    printf("\n");
    printf("Enter hole-fitting algorithm (0=first fit, 1=best_fit):");
    numRead= scanf("%d", &*strategyType);
    while(numRead !=1 || (*strategyType != FIRST_FIT && *strategyType != BEST_FIT))
    {
        printf("Input Error! Please enter a 0 or 1 (0=first fit, 1=best_fit)\n");
        scanf ("%*[^\n]");
        numRead= scanf("%d", &*strategyType);
    }
    printf("\n");
    //Initializing linked list with Dummy node of size 0;
    (*head) = (memoryBlock*)malloc(sizeof(memoryBlock));
    (*head)->next = NULL;
    (*head)->ID = -1;
    (*head)->startingAddressOfBlock=0;
    (*head)->endingAddressOfBlock=0;
    *lastHoleSize = *pmSize;
}//end function enterParameters(...)


//Function purpose to print allocation table
void printAllocationTable(memoryBlock *head)
{
    memoryBlock *tempList = head;
    if(tempList ==NULL)
    {
        
        printf("Allocation table is empty\n");
    }
    else{
    printf("ID\t\tStart\t\t  End\n-------------------------------------\n");
    while(tempList != NULL){
    printf("%d\t  %7d\t  %11d\n",tempList->ID, tempList->startingAddressOfBlock,tempList->endingAddressOfBlock);
        tempList = tempList->next;
    }
  }
}//end function  printAllocationTable(...)




//Function purpose to allocate a new block of memory if physical space is avaialble
void allocateMemory(memoryBlock **head, int *pmSize, int strategyType, int*lastHoleSize)
{
    int tempID= 0, inputBlockSize = 0, numRead= 0, holeSize = 0, prevDiffrence= INT_MAX;
    memoryBlock  *currentBlock= NULL , *newBlock = NULL, *prevBlock=NULL;

    //Checking if linked list allocated
    if(*head== NULL)
    {
        printf("Linked list not allocated\n");
        return;
    }
    
   printf("Enter block ID: ");
   numRead = scanf("%d", &tempID);
    while (numRead!=1 || tempID < 0) {
        printf("Input error, please enter a positive integer: ");
        scanf ("%*[^\n]");
        numRead = scanf("%d", &tempID);
    }
    
    printf("Enter block size: ");
    numRead = scanf("%d" , &inputBlockSize);
    while (numRead != 1 || inputBlockSize <= 0){
        printf("Input error, please enter a positive integer: ");
        scanf("%*[^\n]");
        numRead = scanf("%d" , &inputBlockSize);
    }
    printf("\n");
   
    if(inputBlockSize > *pmSize)
    {
        printf("Block size is larger than remaining unallocated space!\n");
        return;
    }
    newBlock = (memoryBlock*)malloc(sizeof(memoryBlock));
    newBlock->next=NULL;
    newBlock->ID = tempID;
    newBlock->startingAddressOfBlock = 0;
    newBlock->endingAddressOfBlock = inputBlockSize;
    
    
   
    
   //checking if only DUMMY node exists in list
   if((*head)->ID == -1)
   {
       *head =newBlock;
       *pmSize-= inputBlockSize;
       *lastHoleSize -=inputBlockSize;
   }
   else if(idExists(*head ,tempID))
    {
           printf("ID already exists, request rejected!\n");
           return;
    }
   else if(strategyType == FIRST_FIT)
   {
       *pmSize-= inputBlockSize;
       currentBlock =(*head);
     
       while( currentBlock->next != NULL && holeSize < inputBlockSize)
       {
           holeSize = currentBlock->next->startingAddressOfBlock - currentBlock->endingAddressOfBlock;
           prevBlock = currentBlock;
           currentBlock = currentBlock->next;
       }
    
     if(holeSize >= inputBlockSize)
     {
         newBlock->startingAddressOfBlock = prevBlock->endingAddressOfBlock;
         newBlock->endingAddressOfBlock += newBlock->startingAddressOfBlock;
         
         newBlock->next = prevBlock->next;
         prevBlock->next = newBlock;
        
     }
      else{
         newBlock->startingAddressOfBlock = currentBlock->endingAddressOfBlock ;
         newBlock->endingAddressOfBlock += newBlock->startingAddressOfBlock;
         currentBlock->next = newBlock;
         *lastHoleSize -=inputBlockSize;
         }
      
   }//End else first fit
    
   else //Best Fit
   {
       currentBlock = (*head);
       *pmSize-= inputBlockSize;
       
       while(currentBlock->next != NULL)
       {
           holeSize = currentBlock->next->startingAddressOfBlock - currentBlock->endingAddressOfBlock;
           if(holeSize  >= inputBlockSize)
           {
               if((holeSize - inputBlockSize) < prevDiffrence)
               {
                   prevDiffrence = holeSize - inputBlockSize;
                   prevBlock = currentBlock;
               }
               
           }
        
           currentBlock = currentBlock->next;
       }
       
       if(prevBlock == NULL )
       {
           newBlock->startingAddressOfBlock = currentBlock->endingAddressOfBlock ;
           newBlock->endingAddressOfBlock += newBlock->startingAddressOfBlock;
           currentBlock->next = newBlock;
           *lastHoleSize -=inputBlockSize;
       }
       else if((*lastHoleSize-inputBlockSize < prevDiffrence) && (*lastHoleSize >= inputBlockSize))
       {
           newBlock->startingAddressOfBlock = currentBlock->endingAddressOfBlock ;
           newBlock->endingAddressOfBlock += newBlock->startingAddressOfBlock;
           currentBlock->next = newBlock;
           *lastHoleSize -=inputBlockSize;
       }
       else{
           newBlock->startingAddressOfBlock = prevBlock->endingAddressOfBlock;
           newBlock->endingAddressOfBlock += newBlock->startingAddressOfBlock;
           
           newBlock->next = prevBlock->next;
           prevBlock->next = newBlock;
       }

   }//end else Best fit
    
}//end function allocateMemory(...)

//Function purpose to compaact the memory blocks to be contiguous
void defragmentMemory(memoryBlock **head)
{
    int currentHoleSize =0;
    memoryBlock * currentBlock= (*head), * prevBlock=NULL;
    
    while(currentBlock->next!= NULL)
    {
        prevBlock= currentBlock;
        currentBlock = currentBlock->next;
        
        //Checking if hole size is != 0
      if((currentBlock->startingAddressOfBlock - prevBlock->endingAddressOfBlock)!= 0)
         {
             currentHoleSize =(currentBlock->startingAddressOfBlock - prevBlock->endingAddressOfBlock);
             currentBlock->startingAddressOfBlock -= currentHoleSize;
             currentBlock->endingAddressOfBlock -= currentHoleSize;
            
          }
    }//end while
    
}//end defragmentMemory(...)


//Function prpose to dealocate memory block given block ID
void dealocateMemory(memoryBlock **head, int *pmSize,int* lastHoleSize)
{
    int numRead=0, blockID =0;
    memoryBlock* currentBloc =(*head), *prevBlock=NULL;
   
    printf("Enter block ID: ");
    numRead = scanf("%d", &blockID);
    while(numRead != 1|| blockID < 0)
    {
        printf("Input error!\nEnter block ID:");
        scanf("%*[^\n]");
        numRead = scanf("%d", &blockID);
    }
    
    if(currentBloc != NULL && currentBloc->ID == blockID)
    {  *pmSize += (*head)->endingAddressOfBlock;
       *head= currentBloc->next;
        free(currentBloc);
        currentBloc =NULL;
        return;
    }
    while(currentBloc != NULL && currentBloc->ID != blockID)
    {
        prevBlock= currentBloc;
        currentBloc = currentBloc->next;
    }
    
    if(currentBloc == NULL)
    {
        printf("Block ID not found\n");
        return;
    }
    else{
        //If end ID is found at last element and dealocated memory to the last memory size
        if(currentBloc->next == NULL)
        {
            *lastHoleSize += (currentBloc->endingAddressOfBlock-currentBloc->startingAddressOfBlock);
        }
        *pmSize += (currentBloc->endingAddressOfBlock-currentBloc->startingAddressOfBlock);
        prevBlock->next =currentBloc->next;
        free(currentBloc);
        currentBloc = NULL;
    }
    
}//End function void dealocateMemory(memoryBlock **head)



//function purpose to ceck if ID already exists in List
bool idExists(memoryBlock * head, int inputID)
{
    memoryBlock * current = head;
    while(current != NULL)
    {
        if(current->ID == inputID)
            return true;
        current = current->next;
    }
    return false;
}//End function check ID



//Function purpose to free all allocated memory
void destroy(memoryBlock **head)
{
 if(!*head)
     return;
    destroy(&((*head)->next));
    free((*head));
    *head= NULL;
}//end function
