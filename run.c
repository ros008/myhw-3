#include <sys/types.h>
#include <limits.h>

#include "run.h"
#include "util.h"

#include <stdio.h>

void *base = 0;
//p_meta last = NULL;
void *last = 0;
// void 

p_meta find_meta(p_meta *last, size_t size) {

	int best_size = -1;

  	p_meta index = base;
	p_meta result = base;

  	switch(fit_flag){
    		case FIRST_FIT:
    		{
      			//FIRST FIT CODE
			while(index) {
				if(index -> free == 1 && index -> size >= size) {
					result = index;
					break;
				}
				index = index -> next;	
			}
    		}
    		break;

    		case BEST_FIT:
    		{
      			//BEST_FIT CODE
			while(index) {
				if(index -> free == 1 && index -> size >= size) {
					if(best_size == -1 || index -> size < best_size) {
						result = index;
						best_size = index -> size;
					}
				}
				index = index -> next;
			}
    		}
    		break;

    		case WORST_FIT:
    		{
      			//WORST_FIT CODE
			while(index) {
                                if(index -> free == 1 && index -> size >= size) {
                                        if(best_size == -1 || index -> size > best_size) {
                                                result = index;
                                                best_size = index -> size;
                                        }
                                }
                                index = index -> next;
                        }
    		}
    		break;

  	}	
  	return result;
}

void *m_malloc(size_t size) {
	p_meta ftmp;
	
	if(size == 0) {
		return NULL;	
	}	
	if(size %4) {
		size = size + (4- size%4);
	}

	ftmp = find_meta(last, size); ///////////  & need????
	
	if(ftmp) { // if found

		if(ftmp->size > size + META_SIZE) {

	
		
			//tmp -> free = 0;
			p_meta new = ftmp + size; 
			new -> next = ftmp -> next;
			new -> prev = ftmp;
			ftmp -> next ->prev = new; 
			ftmp -> next = new;
		
			new -> free = 1;
			new -> size = ftmp->size -(size+META_SIZE);
			//		
			// ftmp->size = size + META_SIZE;
		}

		ftmp -> free = 0;		
		
	}
	else { // not found - need new block

		if (sbrk(META_SIZE + size) == -1) {  // no space
			return (void*)0;
		}
		if(base == 0) {
			base = sbrk(0);
			ftmp -> prev = NULL;
			ftmp -> next = NULL;
			ftmp -> size = size;
			last = ftmp;
			ftmp -> free = 0;	
		}
		else {
			ftmp = sbrk(META_SIZE + size);
        	        ftmp -> prev = last;
	                ftmp -> next = NULL;
			p_meta f_meta = last;
			f_meta -> next = ftmp;
			last = ftmp;
			ftmp -> free = 0;
		}
		
		
	}
	
}

void m_free(void *ptr) {

	if (ptr == NULL) {
		return NULL;
	}

	//p_meta ftmp = base;
	
	p_meta addr = ptr - META_SIZE;	

	addr -> free = 1;
	
	if(addr->prev != 0) { // addr is not first
		if(addr -> prev -> free == 1) {
			addr -> prev -> size = (addr->prev->size) + addr->size + META_SIZE;
			//addr -> prev -> next = addr -> next;
			if(addr -> next != 0) {
				addr -> prev -> next = addr -> next;	
				addr -> next -> prev = addr -> prev;
			}
			else {
				addr -> prev -> next = NULL;
				last = addr-> prev;
			}
		}
		//addr->prev->size = (addr->prev->size) + addr->size + META_SIZE;
		//addr->prev->next = addr->next;
		//addr -> next -> prev = addr -> prev;
	}
	/*
	else {
		
		
	}
	*/

	if(addr -> next != 0) { // addr is not last
		if(addr -> next -> free == 1) {
			addr -> size = addr -> size + addr-> next->size + META_SIZE;			

			
			if(addr -> next -> next != 0) {
				addr -> next = addr-> next->next;
				addr->next->next->prev = addr;
			}
			else {
				addr -> next = NULL;
				last = addr;
				
			}	
			
		}

	}
	
	if (addr -> prev == NULL && addr->next == NULL) {
		last = 0;
		base = 0;
	}
		
}

void* m_realloc(void* ptr, size_t size) {




	
}









