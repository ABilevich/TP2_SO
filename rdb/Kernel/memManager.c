#include <memInter.h>

#define HEAP_START 0x600000
#define TOTAL_HEAP_SIZE 1024*1024

#ifdef BUDDY

#else

typedef struct block_link{
	struct block_link *next_free_block;	    /*<< The next free block in the list. */
	int block_size;						/*<< The size of the free block. */
} block_link;

static void insertBlockIntoFreeList(block_link* block_to_insert);
static void HeapInit();
void _t_printFreeMem();

static const uint16_t heap_struct_size	= sizeof(block_link);
#define HEAP_MINIMUM_BLOCK_SIZE	( ( int ) ( heap_struct_size * 2 ) )

static block_link Start, End;
static int free_bytes_remaining = TOTAL_HEAP_SIZE;
static int minimum_ever_free_bytes_remaining = 0U;
static int number_of_successful_allocations = 0;
static int number_0f_successful_frees = 0;


void malloc(int  wanted_size, void ** ret_val){
    printString("M", 1);
    printNewLine();	
    
    block_link *block, *previous_block, *new_block_link;
    static int HeapHasBeenInitialised = 0;
   
    if( HeapHasBeenInitialised == 0 ){
        HeapInit();
        HeapHasBeenInitialised = 1;
    }
    _t_printFreeMem();
    if( wanted_size > 0 ){
		wanted_size += heap_struct_size;
    }
    if( ( wanted_size > 0 ) && ( wanted_size < TOTAL_HEAP_SIZE ) ){
        /* Blocks are stored in byte order - traverse the list from the start
        (smallest) block until one of adequate size is found. */
        previous_block = &Start;
        block = Start.next_free_block;
        while( ( block->block_size < wanted_size ) && ( block->next_free_block != NULL ) )
        {
            previous_block = block;
            block = block->next_free_block;
        }

        /* If we found the end marker then a block of adequate size was not found. */
        if( block != &End )
        {
            /* Return the memory space - jumping over the BlockLink_t structure
            at its start. */
            *ret_val = ( void * ) ( ( ( uint8_t * ) previous_block->next_free_block ) + heap_struct_size );
            
            /* This block is being returned for use so must be taken out of the
            list of free blocks. */
            previous_block->next_free_block = block->next_free_block;

            /* If the block is larger than required it can be split into two. */
            if( ( block->block_size - wanted_size ) > HEAP_MINIMUM_BLOCK_SIZE )
            {
                /* This block is to be split into two.  Create a new block
                following the number of bytes requested. The void cast is
                used to prevent byte alignment warnings from the compiler. */
                new_block_link = ( void * ) ( ( ( uint8_t * ) block ) + wanted_size );

                /* Calculate the sizes of two blocks split from the single
                block. */
                new_block_link->block_size = block->block_size - wanted_size;
                block->block_size = wanted_size;

                /* Insert the new block into the list of free blocks. */
                insertBlockIntoFreeList(new_block_link);
            }

            free_bytes_remaining -= block->block_size;

        }else{
            *ret_val = NULL;
        }
        _t_printFreeMem();
	}
}

void free(void * p){
    printString("F", 1);
    printNewLine();
    _t_printFreeMem();
    uint8_t *puc = ( uint8_t * ) p;
    block_link *link;

	if( p != NULL )
	{
		/* The memory being freed will have an BlockLink_t structure immediately
		before it. */
		puc -= heap_struct_size;
        
		/* This unexpected casting is to keep some compilers from issuing
		byte alignment warnings. */
		link = ( void * ) puc;
        printDec(link->block_size);
        printNewLine();
        /* Add this block to the list of free blocks. */
        insertBlockIntoFreeList( ( ( block_link * ) link ) );
        free_bytes_remaining += link->block_size;
	}
    _t_printFreeMem();
}

void getFreeHeapSize(int * resp){
    //printDec(free_bytes_remaining);
	*resp = free_bytes_remaining;
}

void _t_printFreeMem(){
    block_link *aux_block;
    printString("start", 5);
    printNewLine();	
    for( aux_block = &Start; aux_block->next_free_block != NULL; aux_block = aux_block->next_free_block )	{		
        printString("block size: ", 12);																	
        printDec(aux_block->block_size);
        printNewLine();
    }	
    printString("end", 3);
    printNewLine();
	
}

static void HeapInit(){
block_link * first_free_block;

    /* Start is used to hold a pointer to the first item in the list of free
	blocks.  The void cast is used to prevent compiler warnings. */
	Start.next_free_block = ( void * ) HEAP_START;
	Start.block_size = ( int ) 0;

	/* xEnd is used to mark the end of the list of free blocks. */
	End.block_size = TOTAL_HEAP_SIZE;
	End.next_free_block = NULL;

	/* To start with there is a single free block that is sized to take up the
	entire heap space. */
	first_free_block = ( void * ) HEAP_START;
	first_free_block->block_size = TOTAL_HEAP_SIZE;
	first_free_block->next_free_block = &End;
}





static void insertBlockIntoFreeList(block_link* block_to_insert){
    block_link *iterator;															
    int block_size = block_to_insert->block_size;										
                                                                                    
    /* Iterate through the list until a block is found that has a larger size */	
    /* than the block we are inserting. */											
    for( iterator = &Start; iterator->next_free_block->block_size < block_size; iterator = iterator->next_free_block )	{																				
        /* There is nothing to do here - just iterate to the correct position. */	
    }																				
                                                                                    
    /* Update the list to include the block being inserted in the correct */		
    /* position. */																	
    block_to_insert->next_free_block = iterator->next_free_block;					
    iterator->next_free_block = block_to_insert;
}


#endif