#ifndef BUDDY

#include <mm_interface.h>
#define BYTE_ALIGNMENT 8
#define BYTE_ALIGNMENT_MASK 7

typedef struct block_link{
	struct block_link *next_free_block;	    /*<< The next free block in the list. */
	size_t  block_size;						/*<< The size of the free block. */
} block_link;

static void insertBlockIntoFreeList(block_link* block_to_insert);
static void HeapInit();
void _t_printFreeMem();

static const uint16_t heap_struct_size	= ( sizeof( block_link ) + ( ( size_t ) ( BYTE_ALIGNMENT - 1 ) ) ) & ~( ( size_t ) BYTE_ALIGNMENT_MASK );

#define BITS_PER_BYTE   ( ( size_t ) 8 )
#define HEAP_MINIMUM_BLOCK_SIZE ( ( size_t ) ( heap_struct_size << 1 ) )

static block_link Start, *End = NULL;
static size_t  free_bytes_remaining = TOTAL_HEAP_SIZE;
static size_t  number_of_successful_allocations = 0;
static size_t  number_of_successful_frees = 0;

/* Gets set to the top bit of an size_t type.  When this bit in the xBlockSize
member of an BlockLink_t structure is set then the block belongs to the
application.  When the bit is free the block is still part of the free heap
space. */
static size_t block_allocated_bit = 0;

void malloc(size_t wanted_size, void ** ret_val){

    block_link *block, *previous_block, *new_block_link;
   
    if( End == NULL ){
        HeapInit();
    }

    /* Check the requested block size is not so large that the top bit is
	set.  The top bit of the block size member of the BlockLink_t structure
	is used to determine who owns the block - the application or the
	kernel, so it must be free. */
    if( ( wanted_size & block_allocated_bit ) == 0 ){

         /* The wanted size is increased so it can contain a BlockLink_t
	    structure in addition to the requested amount of bytes. */
        if( wanted_size > 0 ){
            
            wanted_size += heap_struct_size;
            
            /* Ensure that blocks are always aligned to the required number
			of bytes. */
            if( ( wanted_size & BYTE_ALIGNMENT_MASK ) != 0x00 ){
                /* Byte alignment required. */
                wanted_size += ( BYTE_ALIGNMENT - ( wanted_size & BYTE_ALIGNMENT_MASK ) );
            }
            
        }

        if( ( wanted_size > 0 ) && ( wanted_size <= free_bytes_remaining ) ){
            /* Traverse the list from the start	(lowest address) block until
            one	of adequate size is found. */
            previous_block = &Start;
            block = Start.next_free_block;
            while( ( block->block_size < wanted_size ) && ( block->next_free_block != NULL ) ){
                previous_block = block;
                block = block->next_free_block;
            }

            /* If we found the end marker then a block of adequate size was not found. */
            if( block != End ){
                /* Return the memory space - jumping over the BlockLink_t structure
                at its start. */
                *ret_val = ( void * ) ( ( ( uint8_t * ) previous_block->next_free_block ) + heap_struct_size );

                /* This block is being returned for use so must be taken out of the
                list of free blocks. */
                previous_block->next_free_block = block->next_free_block;

                /* If the block is larger than required it can be split into two. */
                if( ( block->block_size - wanted_size ) > HEAP_MINIMUM_BLOCK_SIZE ){
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

                /* The block is being returned - it is allocated and owned
				by the application and has no "next" block. */
                block->block_size |= block_allocated_bit;
				block->next_free_block = NULL;
                number_of_successful_allocations++;

            }else{
                *ret_val = NULL;
            }

        }
    }
}

void free(void * p){

    uint8_t *puc = ( uint8_t * ) p;
    block_link *link;

	if( p != NULL ){
		/* The memory being freed will have an BlockLink_t structure immediately
		before it. */
		puc -= heap_struct_size;
        
		/* This unexpected casting is to keep some compilers from issuing
		byte alignment warnings. */
		link = ( void * ) puc;

        if( ( link->block_size & block_allocated_bit ) != 0 ){
            if( link->next_free_block == NULL ){
                /* The block is being returned to the heap - it is no longer
                allocated. */
                link->block_size &= ~block_allocated_bit;

                /* Add this block to the list of free blocks. */
                free_bytes_remaining += link->block_size;
                insertBlockIntoFreeList( ( ( block_link * ) link ) );
                number_of_successful_frees++;
            }
        }
    }

}

void getFreeHeapSize(size_t * resp){
	*resp = free_bytes_remaining;
}

void getTotalHeapSize(size_t * resp){
	*resp = TOTAL_HEAP_SIZE;
}

void getTakenHeapSize(size_t * resp){
	*resp = TOTAL_HEAP_SIZE - free_bytes_remaining;
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
    uint8_t * puc_aligned_heap;
    size_t  address = HEAP_START;
    size_t total_heap_size = TOTAL_HEAP_SIZE;

	/* Ensure the heap starts on a correctly aligned boundary. */

    if( ( address & BYTE_ALIGNMENT_MASK ) != 0 ){
		address += ( BYTE_ALIGNMENT - 1 );
		address &= ~( ( size_t ) BYTE_ALIGNMENT_MASK );
		total_heap_size -= address - ( size_t ) HEAP_START;
	}

    puc_aligned_heap = ( uint8_t * ) address;

    /* Start is used to hold a pointer to the first item in the list of free
	blocks.  The void cast is used to prevent compiler warnings. */
	Start.next_free_block = ( void * ) puc_aligned_heap;
	Start.block_size = ( size_t  ) 0;

	/* End is used to mark the end of the list of free blocks and is inserted
	at the end of the heap space. */
    address = ((size_t) puc_aligned_heap) + total_heap_size;
    address -= heap_struct_size;
    address &= ~( ( size_t ) BYTE_ALIGNMENT_MASK );
    End = (void *) address;
	End->block_size = 0;
	End->next_free_block = NULL;

	/* To start with there is a single free block that is sized to take up the
	entire heap space, minus the space taken by End. */
	first_free_block = ( void * ) puc_aligned_heap;
	first_free_block->block_size = address - (size_t) first_free_block;
	first_free_block->next_free_block = End;
    
    /* Only one block exists - and it covers the entire usable heap space. */
    free_bytes_remaining = first_free_block->block_size;

    /* Work out the position of the top bit in a size_t variable. */
	block_allocated_bit = ( ( size_t ) 1 ) << ( ( sizeof( size_t ) * BITS_PER_BYTE ) - 1 );
}





static void insertBlockIntoFreeList(block_link* block_to_insert){
    block_link *iterator;
    uint8_t *puc;																								
                                                                                    
    /* Iterate through the list until a block is found that has a higher address
	than the block being inserted. */											
    for( iterator = &Start; iterator->next_free_block < block_to_insert; iterator = iterator->next_free_block )	{																				
        /* There is nothing to do here - just iterate to the correct position. */	
    }	

    /* Do the block being inserted, and the block it is being inserted after
	make a contiguous block of memory? */
	puc = ( uint8_t * ) iterator;
	if( ( puc + iterator->block_size ) == ( uint8_t * ) block_to_insert ){
		iterator->block_size += block_to_insert->block_size;
		block_to_insert = iterator;
	}


    /* Do the block being inserted, and the block it is being inserted before
	make a contiguous block of memory? */
	puc = ( uint8_t * ) block_to_insert;
	if( ( puc + block_to_insert->block_size ) == ( uint8_t * ) iterator->next_free_block ){
		if( iterator->next_free_block != End ){
			/* Form one big block from the two blocks. */
			block_to_insert->block_size += iterator->next_free_block->block_size;
			block_to_insert->next_free_block = iterator->next_free_block->next_free_block;
		}
		else{
			block_to_insert->next_free_block = End;
		}
	}
	else{
		block_to_insert->next_free_block = iterator->next_free_block;
	}
                                                                          
    /* Update the list to include the block being inserted in the correct */		
    /* position. */	
    /* If the block being inserted plugged a gab, so was merged with the block
	before and the block after, then it's pxNextFreeBlock pointer will have
	already been set, and should not be set here as that would make it point
	to itself. */
	if( iterator != block_to_insert ){
		iterator->next_free_block = block_to_insert;
	}																				
    
}

#endif
