#include "../codexion.h"

/* DAY 7: Compare two requests according to FIFO or EDF. */
int	request_has_priority(t_heap *heap, t_request *first, t_request *second)
{
	if (heap->scheduler == 1)
	{
		if (first->request_time != second->request_time)
			return (first->request_time < second->request_time);
	}
	else
	{
		if (first->deadline != second->deadline)
			return (first->deadline < second->deadline);
	}
	return (first->coder->id < second->coder->id);
}

/* DAY 7: Initialize an empty priority queue for one dongle. */
int	heap_init(t_heap *heap, int capacity, int scheduler)
{
	heap->requests = malloc(sizeof(t_request) * capacity);
	if (!heap->requests)
		return (1);
	heap->size = 0;
	heap->capacity = capacity;
	heap->scheduler = scheduler;
	return (0);
}

/* DAY 7: Swap two requests inside the heap. */
void	heap_swap(t_request *first, t_request *second)
{
	t_request	temp;

	temp = *first;
	*first = *second;
	*second = temp;
}

/* DAY 7: Restore heap order after adding a request. */
void	heap_push(t_heap *heap, t_request request)
{
	int	index;
	int	parent;

	if (heap->size >= heap->capacity)
		return ;
	index = heap->size;
	heap->requests[index] = request;
	heap->size++;
	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!request_has_priority(heap, &heap->requests[index],
				&heap->requests[parent]))
			break ;
		heap_swap(&heap->requests[index], &heap->requests[parent]);
		index = parent;
	}
}

/* DAY 7: Return the highest-priority request without removing it. */
t_request	*heap_peek(t_heap *heap)
{
	if (heap->size == 0)
		return (NULL);
	return (&heap->requests[0]);
}

/* DAY 7: Remove and return the highest-priority request. */
t_request	heap_pop(t_heap *heap)
{
	t_request	result;
	int			index;
	int			child;

	result = heap->requests[0];
	heap->size--;
	heap->requests[0] = heap->requests[heap->size];
	index = 0;
	while (index * 2 + 1 < heap->size)
	{
		child = index * 2 + 1;
		if (child + 1 < heap->size
			&& request_has_priority(heap, &heap->requests[child + 1],
				&heap->requests[child]))
			child++;
		if (!request_has_priority(heap, &heap->requests[child],
				&heap->requests[index]))
			break ;
		heap_swap(&heap->requests[index], &heap->requests[child]);
		index = child;
	}
	return (result);
}