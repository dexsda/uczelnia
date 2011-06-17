#define MAX 100

void heapify (int * heap, int i, int elements) {
	int maxi=i;
	if(i<elements && heap[maxi]<heap[i*2+1])
		maxi=i*2+1;
	if(i<elements && heap[maxi]<heap[i*2+2])
		maxi=i*2+2;
	if(maxi!=i){
		int temp = heap[maxi];
		heap[maxi] = heap[i];
		heap[i] = temp;
		heapify(heap,maxi,elements);
	}
}

int delmax (int *heap, int elements) {
	int max=-1;
	if(elements>0){
		max=heap[0];
		elements--;
		heap[0]=heap[elements];
		heapify(heap,0,elements);
	}
	return max;
}

int retmax (int *heap, int elements) {
	int max=-1;
	if(elements>0)
		max=heap[0];
	return max;
}

int add (int *heap, int elements, int newval){
	if(elements<MAX){
		int i=elements++;
		while(i>0 && heap[(i-1)/2]<newval){
			heap[i]=heap[(i-1)/2];
			i=(i-1)/2;
		}
		heap[i]=newval;
		return 0;
	} else {
		return -1;
	}
}
